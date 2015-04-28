#ifndef OPTIONS
#define OPTIONS
#include <string>

class options {
public:
	double kB;           // Boltzmann constant in J/K
	double amu;          // Atomic mass unit in kg
	double LJEpsilon;    // Lennard-Jones Potential Parameter in J (http://dx.doi.org/10.1063/1.479848)
	double LJSigma;      // Lennard-Jones Potential Parameter in m (http://dx.doi.org/10.1063/1.479848)
	double mass;         // Mass of particles in kg
	double temp;         // Temperature in K
	int L;               // Number of times to repeat lattice in each dir
	int nSteps;          // Number of time steps
	double dt;           // Time change between steps in s
	bool seedflag;       // Whether we provide a RNG seed or not
	unsigned int seed;   // RNG seed
	int tstatStep;       // How often to run the thermostat
	int tstatStop;       // When to stop running the thermostat
	int tstatStart;      // When to start running the thermostat
	std::string output;  // Name of output file

	options();

	void parseLine(const std::string& s);
};
#endif // OPTIONS
