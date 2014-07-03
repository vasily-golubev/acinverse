#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "seismicdata.h"

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
	SeismicData data;
	data.skipHeader(&ifs);
	data.setDimensions(&ifs);
	data.setDXYT(&ifs);
	data.readData(&ifs);
	data.print();
	cout << "Value: " << data.getValue(0.0, 0.0, 0.4) << endl;
	ifs.close();
	return EXIT_SUCCESS;
}
