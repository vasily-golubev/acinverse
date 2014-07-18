#include "seismicdata.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "types.h"

SeismicData::SeismicData() {
	n_x = n_y = n_z = n_t = 0;
	d_x = d_y = d_z = d_t = 0;
}

SeismicData::~SeismicData() {
	free(data);
	free(image);
}

void SeismicData::generatePointSource(float x, float y, float z, float c) {
	vector3 r_o(x, y, z);
	for (int k = 0; k < n_t; k++)
	for (int j = 0; j < n_y; j++)
	for (int i = 0; i < n_x; i++) {
		vector3 r(i * d_x, j * d_y, 0);
		int ind = i + j * n_x + k * n_x * n_y;
		if ( (fabs(distance(r_o, r) / c - k * d_t) < d_t)
			&& (distance(r_o, r) / c < k * d_t) ) {
			data[ind] = 1.0 / 4.0 / M_PI / distance(r_o, r);
//#define MY_INFINITY 1.0
//			data[ind] = MY_INFINITY;
			std::cout << "Time " << k * d_t << ", Value " << data[ind]
			<< " " << i << " " << j << std::endl;
		} else
			data[ind] = 0.0;
	}
}

void SeismicData::generatePointRickerWavelet(float x, float y, float z, float c, float f) {
	// TODO http://subsurfwiki.org/wiki/Ricker_wavelet.
        vector3 r_o(x, y, z);
        for (int k = 0; k < n_t; k++)
        for (int j = 0; j < n_y; j++)
        for (int i = 0; i < n_x; i++) {
                vector3 r(i * d_x, j * d_y, 0);
                int ind = i + j * n_x + k * n_x * n_y;
		float t = 2.0 * distance(r_o, r) / c;
		float value = (1 - 2.0 * M_PI * M_PI * f * f * t * t) * exp(- M_PI * M_PI * f * f * t * t);
		data[ind] = 1.0 / 4.0 / M_PI / distance(r_o, r) * value;
        }
}

/*void SeismicData::setDimensions(int n_x, int n_y, int n_t) {
	this->n_x = n_x;
	this->n_y = n_y;
	this->n_z = n_z;
	this->n_t = n_t;
}*/

void SeismicData::setDimensions(std::ifstream *ifs) {
	(*ifs) >> this->n_t;
	(*ifs) >> this->n_x;
	(*ifs) >> this->n_y;
	(*ifs) >> this->n_z;
	data = (float *) malloc(n_x * n_y * n_t * sizeof(float));
	if (!data)
			throw std::runtime_error("Can't allocate memory for data.");
	image = (float *) malloc(n_x * n_y * n_z * sizeof(float));
	if (!image)
			throw std::runtime_error("Can't allocate memory for image.");
}

void SeismicData::setDXYZT(std::ifstream *ifs) {
	(*ifs) >> this->d_t;
	(*ifs) >> this->d_x;
	(*ifs) >> this->d_y;
	(*ifs) >> this->d_z;
}

void SeismicData::readData(std::ifstream *ifs) {
	for (int k = 0; k < n_t; k++)
	for (int j = 0; j < n_y; j++)
	for (int i = 0; i < n_x; i++) {
		int ind = i + j * n_x + k * n_x * n_y;
		(*ifs) >> data[ind];
	}
}

float SeismicData::getValue(float x, float y, float t) {
	int i = x / d_x;
	int j = y / d_y;
	int k = t / d_t;
	if ( ((i < 0) || (i > n_x - 1)) ||
		((j < 0) || (j > n_y - 1)) ||
		((k < 0) || (k > n_t - 1)) ) {
			throw std::runtime_error("For migration longer seismogram is necessary.");
		}
	int ind = i + j * n_x + k * n_x * n_y;
	return data[ind];
}

void SeismicData::printDimensions() {
	std::cout << "N_X = " << n_x << ", N_Y = " << n_y << ", N_Z = " << n_z
			<< ", N_T = " << n_t << std::endl;
}

void SeismicData::print() {
	std::cout << "N_X = " << n_x << ", N_Y = " << n_y << ", N_Z = " << n_z
			<< ", N_T = " << n_t << std::endl;
	std::cout << "d_x = " << d_x << ", d_y = " << d_y << ", d_z = " << d_z
		<< ", d_t = " << d_t << std::endl;
	for (int k = 0; k < n_t; k++) {
		for (int j = 0; j < n_y; j++) {
			for (int i = 0; i < n_x; i++) {
				int ind = i + j * n_x + k * n_x * n_y;
				std::cout << data[ind] << "\t";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

void SeismicData::skipHeader(std::ifstream *ifs) {
	const int magic_number = 9;
	char str[256];
	for (int i = 0; i < magic_number; i++)
		ifs->getline(str, 256);
}

void SeismicData::saveImage() {
	std::ofstream ofs;
	ofs.open(filename, std::ofstream::out);
	if (ofs.fail()) {
		std::cout << "Can't open " << filename << " file." << std::endl;
		return;
	}
	/* Make header. */
	ofs << "# vtk DataFile Version 3.0\n";
	ofs << "Created by AC_INVERSE\n";
	ofs << "ASCII\n";
	ofs << "DATASET STRUCTURED_POINTS\n";
	ofs << "DIMENSIONS " << n_x << " " << n_y << " " << n_z << "\n";
	ofs << "SPACING " << d_x << " " << d_y << " " << d_z << "\n";
	ofs << "ORIGIN 0 0 0\n";
	ofs << "POINT_DATA " << n_x * n_y * n_z << "\n";
	ofs << "SCALARS Um float 1\n";
	ofs << "LOOKUP_TABLE Um_table \n";
	/* Save image. */
	for (int k = 0; k < n_z; k++) {
		for (int j = 0; j < n_y; j++)
		for (int i = 0; i < n_x; i++) {
			int ind = i + j * n_x + k * n_x * n_y;
			ofs << image[ind] << " ";
		}
		ofs << std::endl;
	}
	ofs.close();
}

void SeismicData::saveSeismograms(const char *name) {
	std::ofstream ofs;
	ofs.open(name, std::ofstream::out);
	if (ofs.fail()) {
		std::cout << "Can't open " << name << " file." << std::endl;
		return;
	}
	/* Make header. */
	ofs << "# vtk DataFile Version 3.0\n";
	ofs << "Created by AC_INVERSE\n";
	ofs << "ASCII\n";
	ofs << "DATASET STRUCTURED_POINTS\n";
	ofs << "DIMENSIONS " << n_x << " " << n_y << " " << n_t << "\n";
	ofs << "SPACING " << d_x << " " << d_y << " " << d_t << "\n";
	ofs << "ORIGIN 0 0 0\n";
	ofs << "POINT_DATA " << n_x * n_y * n_t << "\n";
	ofs << "SCALARS U float 1\n";
	ofs << "LOOKUP_TABLE U_table \n";
	/* Save image. */
	for (int k = 0; k < n_t; k++) {
		for (int j = 0; j < n_y; j++)
		for (int i = 0; i < n_x; i++) {
			int ind = i + j * n_x + k * n_x * n_y;
			ofs << data[ind] << " ";
		}
		ofs << std::endl;
	}
	ofs.close();
}
