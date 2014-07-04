#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "rayleighintegral.h"

using namespace std;

ifstream ifs;

// FIXME Add correct error handling.
int main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "Usage ./kirchhoff_migration input_file_name" << endl;
		return EXIT_FAILURE;
	}
	ifs.open(argv[1], ifstream::in);
	if (ifs.fail()) {
		cout << "Can't open " << argv[1] << " file." << endl;
		return EXIT_FAILURE;
	}
	SeismicData *data = new SeismicData();
	data->skipHeader(&ifs);
	data->setDimensions(&ifs);
	data->setDXYZT(&ifs);
	data->readData(&ifs);
	//data->print();
	//cout << "Value: " << data->getValue(1000.0, 1000.0, 1.0) << endl;
	ifs.close();
	RayleighIntegral integral;
	integral.initParams(2000.0);
	integral.initSeismicData(data);
	//cout << "Integral: " << integral.calculateIntegral(1000, 1000.0, 500.0) << endl;
	integral.calculateImage();
	return EXIT_SUCCESS;
}
