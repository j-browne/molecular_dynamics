#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "vec3.h"
#include "particle.h"
#include "rand_tools.h"

using namespace std;

ostream& operator<< (ostream& os, const vec3& v) {
	os << v.getX() << "\t" << v.getY() << "\t" << v.getZ();
	return os;
}

int main (int argc, char* argv[]) {
	// Options
	int L = 2; // number of times to repeat lattice in each dir
	double b = 1; // size of lattice
	vector<particle> ps;

	// Seed RNG
	// TODO: allow seed input
	srand(time(NULL));

	// Set up initial conditions
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
				float mass = 1;
				float beta = 1;
				float stddev = mass * beta;
				p1.setVel(vec3(rand_gaus(0,stddev),rand_gaus(0,stddev),rand_gaus(0,stddev)));
				p2.setVel(vec3(rand_gaus(0,stddev),rand_gaus(0,stddev),rand_gaus(0,stddev)));
				p3.setVel(vec3(rand_gaus(0,stddev),rand_gaus(0,stddev),rand_gaus(0,stddev)));
				p4.setVel(vec3(rand_gaus(0,stddev),rand_gaus(0,stddev),rand_gaus(0,stddev)));

				// Determine center of mass velocity
				

				ps.push_back(p1);
				ps.push_back(p2);
				ps.push_back(p3);
				ps.push_back(p4);
			}
		}
	}
	// Subtract center of mass velocity

	for (vector<particle>::iterator it = ps.begin(); it != ps.end(); ++it) {
		cout << it->getPos() << '\t' << it->getVel() << endl;
	}

	// Do simulation
}
