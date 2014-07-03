#include "seismicdata.h"

#include <iostream>
#include <string>

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

void SeismicData::printDimensions() {
	std::cout << "N_X = " << n_x << ", N_Y = " << n_y << ", N_T = " << n_t << std::endl;
}

void SeismicData::skipHeader(std::ifstream *ifs) {
	const int magic_number = 7;
	char str[256];
	for (int i = 0; i < magic_number; i++)
		ifs->getline(str, 256);
}
