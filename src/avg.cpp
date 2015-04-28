#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cmath>
using namespace std;

double mean(const vector<double>& v) {
	double s = 0;
	for (vector<double>::const_iterator it = v.begin(); it != v.end(); ++it) {
		s += *it;
	}
	return s/v.size();
}

double stddev(const vector<double>& v, double m) {
	double s = 0;
	for (vector<double>::const_iterator it = v.begin(); it != v.end(); ++it) {
		s += pow(*it - m, 2);
	}
	return sqrt(s/v.size());
}

int main (int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "wrong number of arguments" << endl;
		return 1;
	}

	string finname = argv[1];
	string foutname = finname+".avg";
	double start = atof(argv[2]);

	ifstream infile(finname.c_str());
	ofstream outfile(foutname.c_str());

	// This assumes constant dt
	string line;
	vector<double> VAll;
	vector<double> KAll;
	vector<double> TAll;
	vector<double> PAll;
	vector<double> DAll;

	// Load in values
	while(getline(infile, line)) {
		stringstream ss(line);
		double t, V, K, T, P, D;
		ss >> t >> V >> K >> T >> P >> D;

		if (t >  start) {
			VAll.push_back(V);
			KAll.push_back(K);
			TAll.push_back(T);
			PAll.push_back(P);
			DAll.push_back(D);
		}
	}

	// Calculate mean
	double VMean = mean(VAll);
	double KMean = mean(KAll);
	double TMean = mean(TAll);
	double PMean = mean(PAll);
	double DMean = mean(DAll);

	// Calculate standard deviation
	double VStddev = stddev(VAll, VMean);
	double KStddev = stddev(KAll, KMean);
	double TStddev = stddev(TAll, TMean);
	double PStddev = stddev(PAll, PMean);
	double DStddev = stddev(DAll, DMean);

	outfile << "# V\tdV\tK\tdK\tT\tdT\tP\tdP\tD\tdD" << endl;
	outfile << VMean << "\t" << VStddev << "\t"
	        << KMean << "\t" << KStddev << "\t"
	        << TMean << "\t" << TStddev << "\t"
	        << PMean << "\t" << PStddev << "\t"
	        << DMean << "\t" << DStddev << endl;
}
