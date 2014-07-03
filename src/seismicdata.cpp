#include "seismicdata.h"

#include <iostream>
#include <string>

SeismicData::SeismicData() {
	n_x = n_y = n_t = 0;
	d_x = d_y = d_t = 0;
}

void SeismicData::setDimensions(int n_x, int n_y, int n_t) {
	this->n_x = n_x;
	this->n_y = n_y;
	this->n_t = n_t;
}

void SeismicData::setDimensions(std::ifstream *ifs) {
	(*ifs) >> this->n_t;
	(*ifs) >> this->n_x;
	(*ifs) >> this->n_y;
}

void SeismicData::setDXYT(std::ifstream *ifs) {
	(*ifs) >> this->d_t;
	(*ifs) >> this->d_x;
	(*ifs) >> this->d_y;
}

void SeismicData::readData(std::ifstream *ifs) {
	for (int k = 0; k < n_t; k++)
	for (int j = 0; j < n_y; j++)
	for (int i = 0; i < n_x; i++)
		(*ifs) >> data[i][j][k];
}

float SeismicData::getValue(float x, float y, float t) {
	const float magic_value = -777.777;
	int i = x / d_x;
	int j = y / d_y;
	int k = t / d_t;
	if ( ((i < 0) || (i > n_x - 1)) ||
		((j < 0) || (j > n_y - 1)) ||
		((k < 0) || (k > n_t - 1)) )
		return magic_value;

	return data[i][j][k];
}

void SeismicData::printDimensions() {
	std::cout << "N_X = " << n_x << ", N_Y = " << n_y << ", N_T = " << n_t << std::endl;
}

void SeismicData::print() {
	std::cout << "N_X = " << n_x << ", N_Y = " << n_y << ", N_T = " << n_t << std::endl;
	std::cout << "d_x = " << d_x << ", d_y = " << d_y << ", d_t = " << d_t << std::endl;
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
	const int magic_number = 7;
	char str[256];
	for (int i = 0; i < magic_number; i++)
		ifs->getline(str, 256);
}
