#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "vec3.h"
#include "particle.h"
#include "rand_tools.h"

using namespace std;

void init(vector<particle>& ps, double b, int L);
void sim(vector<particle>& ps, double b, int L);
vec3 forceLJ(const vec3& r, double ep, double sig);

ostream& operator<< (ostream& os, const vec3& v) {
	os << v.getX() << "\t" << v.getY() << "\t" << v.getZ();
	return os;
}

int main (int argc, char* argv[]) {
	// Options
	int L = 2; // number of times to repeat lattice in each dir
	double b = 1; // size of lattice
	vector<particle> ps;

	// TODO: allow seed input
	// Seed RNG
	srand(time(NULL));

	// Set up initial conditions
	init(ps, b, L);

	// Do simulation
	sim(ps, b, L);
}

void init(vector<particle>& ps, double b, int L) {
	double mass = 1;
	double beta = 1;
	double stddev = mass * beta;

	vec3 vcm = vec3(0,0,0); // center of momentum velocity
	double mcm = 0; // total mass

	for (int i = 0; i < L; ++i) {
		for (int j = 0; j < L; ++j) {
			for (int k = 0; k < L; ++k) {
				particle p1,p2,p3,p4;
				// Set initial positions (FCC Lattice)
				p1.setPos(vec3(i*b,j*b,k*b));
				p2.setPos(vec3(i*b,(j+.5)*b,(k+.5)*b));
				p3.setPos(vec3((i+.5)*b,j*b,(k+.5)*b));
				p4.setPos(vec3((i+.5)*b,(j+.5)*b,k*b));
				// Set initial velocities
				p1.setVel(vec3(rand_gaus(0,stddev),rand_gaus(0,stddev),rand_gaus(0,stddev)));
				p2.setVel(vec3(rand_gaus(0,stddev),rand_gaus(0,stddev),rand_gaus(0,stddev)));
				p3.setVel(vec3(rand_gaus(0,stddev),rand_gaus(0,stddev),rand_gaus(0,stddev)));
				p4.setVel(vec3(rand_gaus(0,stddev),rand_gaus(0,stddev),rand_gaus(0,stddev)));

				// Determine center of momentum velocity
				vcm = vcm + p1.getVel() + p2.getVel() + p3.getVel() + p4.getVel();
				mcm = mcm + p1.getMass() + p2.getMass() + p3.getMass() + p3.getMass();

				ps.push_back(p1);
				ps.push_back(p2);
				ps.push_back(p3);
				ps.push_back(p4);
			}
		}
	}
	vcm = vcm/mcm;

	// Subtract center of momentum velocity
	for (vector<particle>::iterator it = ps.begin(); it != ps.end(); ++it) {
		it->setVel(it->getVel() - vcm);
		cout << it->getPos() << '\t' << it->getAcc() << endl;
	}
}

void sim(vector<particle>& ps, double b, int L) {
	int N = 10;
	double dt = .001;

	for (int i = 0; i < N; ++i) {
		cout << endl;
		for (vector<particle>::iterator it = ps.begin(); it != ps.end(); ++it) {
			// Verlet Integration
			it->setVel(it->getVel() + it->getAcc()*dt/2);
			it->setPos(it->getPos() + it->getVel()*dt);
			// Make sure particle stays within boundaries
			vec3 pos = it->getPos();
			for (int j=0; j < 3; ++j) {
				if (pos.get(j) < 0) {
					pos.set(j,pos.get(j)+b*L);
				}
				else if (pos.get(j) > b*L) {
					pos.set(j,pos.get(j)-b*L);
				}
			}
			it->setPos(pos);

			it->setAcc(vec3(0,0,0));
			for (vector<particle>::iterator jt = ps.begin(); jt != ps.end(); ++jt) {
				if (it != jt) {
					// TODO: calculate accel at an earlier point? the positions aren't consistent
					// Take account of periodic boundaries
					// TODO: is this right?
					for (int k = -1; k <= 1; ++k) {
						for (int l = -1; l <= 1; ++l) {
							for (int m = -1; m <= 1; ++m) {
									it->setAcc(it->getAcc() + forceLJ(jt->getPos() + b*L*vec3(k,l,m) - it->getPos(),1,1)/it->getMass());
							}
						}
					}
				}
			}

			it->setVel(it->getVel() + it->getAcc()*dt/2);

			cout << it->getPos() << '\t' << it->getAcc() << endl;
		}
	}
}

// Lennard-Jones Force
vec3 forceLJ(const vec3& r, double ep, double sig) {
	return -24*ep*(2*(pow(sig,12)/pow(r.mag(),13)-pow(sig,6)/pow(r.mag(),7)))*r.unit();
}
