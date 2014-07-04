#ifndef ACINVERSE_RAYLEIGH_INTEGRAL_H
#define ACINVERSE_RAYLEIGH_INTEGRAL_H

#include "seismicdata.h"
#include "types.h"

class RayleighIntegral {
	public:
		~RayleighIntegral();
		void initParams(float c);
		void initSeismicData(SeismicData *sd);
		float calculateIntegral(float x, float y, float z);
		void calculateImage();
	private:
		float uIntegral(vector3 r_c);
		float c;
		SeismicData *sd;
};

#endif
