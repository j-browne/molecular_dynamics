#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "vec3.h"
#include "particle.h"
#include "rand_tools.h"
#include "options.h"

using namespace std;

options ops;

void init(vector<particle>& ps);
void sim(vector<particle>& ps);
double potLJ(const vec3& r, double ep, double sig);
vec3 forceLJ(const vec3& r, double ep, double sig);
void scaleVel(vector<particle>& ps, double scale);
void randVel(vector<particle>& ps, double temp);

ostream& operator<< (ostream& os, const vec3& v) {
	os << v.getX() << "\t" << v.getY() << "\t" << v.getZ();
	return os;
}

int main (int argc, char* argv[]) {
	vector<particle> ps;

	if (argc != 2) {
		cerr << "wrong number of arguments" << endl;
		return 1;
	}

	ifstream infile(argv[1]);

	// Options
	string line;
	while (getline(infile,line)) {
		ops.parseLine(line);
	}

	// Seed RNG
	if (ops.seedflag) {
		srand(ops.seed);
	} else {
		srand(time(NULL));
	}

	// Set up initial conditions
	init(ps);

	// Do simulation
	sim(ps);
}

void init(vector<particle>& ps) {
	double b = 1.22*ops.LJSigma*sqrt(2);

	for (int i = 0; i < ops.L; ++i) {
		for (int j = 0; j < ops.L; ++j) {
			for (int k = 0; k < ops.L; ++k) {
				particle p1,p2,p3,p4;
				// Set masses
				p1.setMass(ops.mass);
				p2.setMass(ops.mass);
				p3.setMass(ops.mass);
				p4.setMass(ops.mass);
				// Set initial positions (FCC Lattice)
				p1.setPos(vec3(i*b,j*b,k*b));
				p2.setPos(vec3(i*b,(j+.5)*b,(k+.5)*b));
				p3.setPos(vec3((i+.5)*b,j*b,(k+.5)*b));
				p4.setPos(vec3((i+.5)*b,(j+.5)*b,k*b));

				ps.push_back(p1);
				ps.push_back(p2);
				ps.push_back(p3);
				ps.push_back(p4);
			}
		}
	}

	randVel(ps, ops.temp);
}

void sim(vector<particle>& ps) {
	ofstream outfile(ops.output.c_str());
	vector<particle> oldPs = ps;         // Starting conditions, needed for diffusion constant
	double b = 1.22*ops.LJSigma*sqrt(2); // Lattice length
	double t = 0;                        // Time in s

	for (int i = 0; i < ops.nSteps; ++i) {
		double KE = 0;    // Kinetic Energy in J
		double r2Sum = 0; // Sum of r^2 in m^2
		double V = 0;     // Potential Energy in J
		double W = 0;     // External Work in J, needed for pressure
		int j = 0;

		// Velocity Verlet Integration
		for (vector<particle>::iterator it = ps.begin(); it != ps.end(); ++it) {
			it->setVel(it->getVel() + it->getAcc()*ops.dt/2.);
			vec3 pos = it->getPos() + it->getVel()*ops.dt;
			pos = pos - floor(pos/(b*ops.L))*(b*ops.L); // Make sure particle stays within boundaries
			it->setPos(pos);
		}

		// Calculate Acceleration
		for (vector<particle>::iterator it = ps.begin(); it != ps.end(); ++it) {
			it->setAcc(vec3(0,0,0));
		}

		for (vector<particle>::iterator it = ps.begin(); it != ps.end(); ++it) {
			for (vector<particle>::iterator jt = it +1; jt != ps.end(); ++jt) {
				// Take account of periodic boundaries, use minimum distance
				vec3 r = jt->getPos()-it->getPos();
				r = r - round(r/(b*ops.L)) * (b*ops.L);
				vec3 F = forceLJ(r,ops.LJEpsilon,ops.LJSigma);
				it->setAcc(it->getAcc() + F/it->getMass());
				jt->setAcc(jt->getAcc() - F/jt->getMass());

				// Add up potential energy and external work
				V = V + potLJ(r,ops.LJEpsilon,ops.LJSigma);
				W = W + vec3::dot(it->getPos(),F)/22 + vec3::dot(jt->getPos(),-F)/22;
			}
		}

		for (vector<particle>::iterator it = ps.begin(); it != ps.end(); ++it) {
			it->setVel(it->getVel() + it->getAcc()*ops.dt/2.);

			KE = KE + .5*(it->getMass())*(vec3::dot(it->getVel(),it->getVel()));
			r2Sum = r2Sum + vec3::dot(it->getPos()-oldPs[j].getPos(),it->getPos()-oldPs[j].getPos());
		}

		// Output
		t = t + ops.dt;                            // Time in s
		double T = (2.*KE)/(3.*ps.size()*ops.kB);  // Temperature in K, using Equipartition Theorem
		double P = (ps.size()*ops.kB*T-W/3.)/pow(ops.L*b,3); // Pressure in Pa
		double D = r2Sum/(6.*t*ps.size()); // Diffusion coefficient in m^2/s

		outfile << t << "\t";  // Time
		outfile << V << "\t";  // Potential Energy
		outfile << KE << "\t"; // Kinetic energy
		outfile << T << "\t";  // Temperature
		outfile << P << "\t";  // Pressure
		outfile << D << endl;  // Diffusion coefficient

		// Temperature Scaling Thermostat
		if (i%ops.tstatStep == 0 && i < ops.tstatStop && i > ops.tstatStart) {
			scaleVel(ps, sqrt(ops.temp/T));
		}
	}
}

// Lennard-Jones Potential
double potLJ(const vec3& r, double ep, double sig) {
	return 4.*ep*((pow(sig/r.mag(),12)-pow(sig/r.mag(),6)));
}

// Lennard-Jones Force
vec3 forceLJ(const vec3& r, double ep, double sig) {
	return -24.*ep*(2*(pow(sig,12)/pow(r.mag(),13)-pow(sig,6)/pow(r.mag(),7)))*r.unit();
}

void scaleVel(vector<particle>& ps, double scale) {
	for (vector<particle>::iterator it = ps.begin(); it != ps.end(); ++it) {
		it->setVel(scale*(it->getVel()));
	}
}

void randVel(vector<particle>& ps, double temp) {
	vec3 pcm = vec3(0,0,0); // Total momentum
	vec3 vcm = vec3(0,0,0); // Center of momentum velocity
	double mcm = 0; // Total mass

	// Set vel as random
	for (vector<particle>::iterator it = ps.begin(); it != ps.end(); ++it) {
		double stddev = sqrt(ops.kB*temp/it->getMass());
		it->setVel(vec3(rand_gaus(0,stddev),rand_gaus(0,stddev),rand_gaus(0,stddev)));

		// Determine center of momentum velocity
		pcm = pcm + it->getMom();
		mcm = mcm + it->getMass();
	}

	vcm = pcm/mcm;

	// Subtract center of momentum velocity
	for (vector<particle>::iterator it = ps.begin(); it != ps.end(); ++it) {
		it->setVel(it->getVel() - vcm);
	}
}
