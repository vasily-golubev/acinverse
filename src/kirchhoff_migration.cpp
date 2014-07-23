#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "rayleighintegral.h"

using namespace std;

ifstream ifs;

// FIXME Add correct error handling.
int main(int argc, char *argv[]) {
	if (argc < 3) {
		cout << "Usage ./kirchhoff_migration input_file_name output_file_name.vtk" << endl;
		return EXIT_FAILURE;
	}
	ifs.open(argv[1], ifstream::in);
	if (ifs.fail()) {
		cout << "Can't open " << argv[1] << " file." << endl;
		return EXIT_FAILURE;
	}
	ifs.close();
	SeismicData *data = new SeismicData();
	data->readDataMadagascar("/var/tmp/zhdanov/test/node6/zodata.rsf@", 2001, 251, 0.01, 40);
	data->saveSeismograms("madagascar.vtk");
	data->setFilename(argv[2]);
/*	data->skipHeader(&ifs);
	data->setDimensions(&ifs);
	data->setDXYZT(&ifs);
	data->readData(&ifs);
	data->print();
	cout << "Value: " << data->getValue(1000.0, 1000.0, 1.0) << endl;
	ifs.close();
*/
	/* Hack to init seismogram as point-source generated. */
//	data->generatePointSource(0, 750, 1000, 2000.0);
//	data->saveSeismograms("temp.vtk");
	RayleighIntegral integral;
	integral.initParams(2000.0);
	integral.initSeismicData(data);
	integral.calculateImage();
	data->saveImage();
	return EXIT_SUCCESS;
}
