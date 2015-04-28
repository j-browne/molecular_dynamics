#include "options.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

// Set Defaults
options::options() {
	kB = 1.3806488E-23;
	amu = 1.66053892E-27;
	LJEpsilon = 125.7*kB;
	LJSigma = 3.345E-10;
	mass = 40*amu;
	temp = 300;
	L = 2;
	nSteps = 10000;
	dt = 1E-15;
	seedflag = false;
	tstatStep = 100;
	tstatStop = 5000;
	tstatStart = 0;
	output = "out";
}

void options::parseLine(const std::string& s) {
	std::stringstream ss(s);
	std::string key;
	ss >> key;
	if (key.compare("LJEpsilon") == 0) {
		ss >> LJEpsilon;
	} else if (key.compare("LJSigma") == 0) {
		ss >> LJSigma;
	} else if (key.compare("mass") == 0) {
		ss >> mass;
	} else if (key.compare("temp") == 0) {
		ss >> temp;
	} else if (key.compare("L") == 0) {
		ss >> L;
	} else if (key.compare("nSteps") == 0) {
		ss >> nSteps;
	} else if (key.compare("dt") == 0) {
		ss >> dt;
	} else if (key.compare("seed") == 0) {
		seedflag = true;
		ss >> seed;
	} else if (key.compare("tstatStep") == 0) {
		ss >> tstatStep;
	} else if (key.compare("tstatStop") == 0) {
		ss >> tstatStop;
	} else if (key.compare("tstatStart") == 0) {
		ss >> tstatStart;
	} else if (key.compare("output") == 0) {
		ss >> output;
	} else {
		std::cerr << "option `" << key << "` not recognized" << std::endl;
	}
}
