#include "rayleighintegral.h"

#include <math.h>
#include <iostream>

RayleighIntegral::~RayleighIntegral() {
	delete sd;
}

void RayleighIntegral::initParams(float c) {
	this->c = c;
}

void RayleighIntegral::initSeismicData(SeismicData *sd) {
	this->sd = sd;
}

float RayleighIntegral::calculateIntegral(float x, float y, float z) {
	vector3 r_c(x, y, z);
	vector3 r_n_c(x, y, z + sd->dZ());
	return -1.0 / 2.0 / M_PI * (uIntegral(r_n_c) - uIntegral(r_c)) / sd->dZ();
}

float RayleighIntegral::uIntegral(vector3 r_c) {
	float result = 0.0;
	for (int j = 0; j < sd->nY(); j++)
	for (int i = 0; i < sd->nX(); i++) {
		vector3 r(i * sd->dX(), j * sd->dY(), 0);
		float d = distance(r_c, r);
		float v = sd->getValue(r.x, r.y, 2.0 * d / c);
		float s = sd->dX() * sd->dY();
		result += v / d * s;
	}
	return result;
}

void RayleighIntegral::calculateImage() {
	// Think about borders!
	// Can't deal with r == r'!!!
	for (int k = 1; k < sd->nZ(); k++) {
		for (int j = 0; j < sd->nY(); j++)
		for (int i = 0; i < sd->nX(); i++) {
			vector3 r_c(i * sd->dX(), j * sd->dY(), k * sd->dZ());
			int ind = i + j * sd->nX() + k * sd->nX() * sd->nY();
			sd->image[ind] = calculateIntegral(r_c.x, r_c.y, r_c.z);
		}
	std::cout << "Z slice number: " << k << std::endl;
	}
}
