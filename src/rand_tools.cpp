#include <cmath>
#include <cstdlib>

const double PI = 4*atan(1);

double rand_gaus(double cent, double stddev) {
	static bool stored = false;
	static double next;
	double r1, r2;

	if (stored) {
		stored = false;
		return next;
	}

	r1 = rand()*(1.0/RAND_MAX);
	r2 = rand()*(1.0/RAND_MAX);

	stored = true;
	next = stddev * sqrt(-2*log(r1)) * cos(2*PI*r2) + cent;
	return stddev * sqrt(-2*log(r1)) * sin(2*PI*r2) + cent;
}
