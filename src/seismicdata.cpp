#include "seismicdata.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

SeismicData::SeismicData() {
	n_x = n_y = n_z = n_t = 0;
	d_x = d_y = d_z = d_t = 0;
	for (int k = 0; k < n_z; k++)
	for (int j = 0; j < n_y; j++)
	for (int i = 0; i < n_x; i++)
		image[i][j][k] = 0.0;
}

void SeismicData::setDimensions(int n_x, int n_y, int n_t) {
	this->n_x = n_x;
	this->n_y = n_y;
	this->n_z = n_z;
	this->n_t = n_t;
}

void SeismicData::setDimensions(std::ifstream *ifs) {
	(*ifs) >> this->n_t;
	(*ifs) >> this->n_x;
	(*ifs) >> this->n_y;
	(*ifs) >> this->n_z;
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
	for (int i = 0; i < n_x; i++)
		(*ifs) >> data[i][j][k];
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

	return data[i][j][k];
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
			for (int i = 0; i < n_x; i++)
				std::cout << data[i][j][k] << "\t";
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
		for (int i = 0; i < n_x; i++)
			ofs << image[i][j][k] << " ";
		ofs << std::endl;
	}
	ofs.close();
}
