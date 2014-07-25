#ifndef ACINVERSE_SEISMIC_DATA_BORN_H
#define ACINVERSE_SEISMIC_DATA_BORN_H

#include <complex>

#include "types.h"

class SeismicDataBorn {
	public:
		SeismicDataBorn();
		~SeismicDataBorn();
		void generateSimpleModel(int nx, float dx, int nz, float dz, float c);
		void generateSimpleData();
		void saveModel(const char *filename);
		void generateData();
		void saveData(const char *filename);
		void migrateData();
	private:
		std::complex<float> calculateMatrixElement(float w, vector3 r_rec, vector3 r_o, vector3 r);
		int n_x;
		int n_z;
		float d_x;
		float d_z;
		float *model;
		std::complex<float> *data_w;
		std::complex<float> *data_t;
		float c_b;
};

#endif
