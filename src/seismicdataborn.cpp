#include <stdexcept>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include "seismicdataborn.h"

SeismicDataBorn::SeismicDataBorn() {
	n_x = n_z = 0;
	d_x = d_z = 0;
}

SeismicDataBorn::~SeismicDataBorn() {
	free(model);
	free(data_w);
	free(data_t);
}

void SeismicDataBorn::generateSimpleModel(int nx, float dx, int nz, float dz, float c) {
	n_x = nx;
	n_z = nz;
	d_x = dx;
	d_z = dz;
	c_b = c;
	model = (float *) calloc(n_x * n_z, sizeof(float));
	for (int i = n_x * 0.25; i <= n_x * 0.75; i++)
		for (int k = n_z * 0.25; k <= n_z * 0.75; k++) {
			int ind = i + k * n_x;
			model[ind] = 0.1;
		}
}

void SeismicDataBorn::generateSimpleData() {
	// Source - at point (0, 0).
	vector3 r_o(0, 0, 0);
	// Moving Omega -250 - +250 Hz with step 0.05 Hz.
	data_w = (std::complex<float> *) calloc(n_x * 10000, sizeof(std::complex<float>));
	// Moving T 0 - 1 s with step 0.001.
	data_t = (std::complex<float> *) calloc(n_x * 10000, sizeof(std::complex<float>));
	float w = -250.0;
	for (int w_n = 1; w_n < 10001; w_n++) {
		w += 0.05;
		std::cout << "Frequence is " << w << " Hz\n";
		for (int rec = 0; rec < n_x; rec++) {
			vector3 r_rec(rec * d_x, 0.0, 0.0);
			std::complex<float> res(0.0, 0.0);
			for (int i = 0; i < n_x; i++)
				for (int k = 0; k < n_z; k++) {
					vector3 r(i * d_x, k * d_z, 0);
					res += calculateMatrixElement(w, r_rec, r_o, r);
				}
			int ind = rec + (w_n - 1) * rec;
			data_w[ind] = res;
		}
	}

	for (int rec = 0; rec < n_x; rec++) {
		std::cout << "Receiver is " << rec << " s\n";
		float t = 0.0;
		for (int t_n = 1; t_n < 1001; t_n++) {
			t += 0.001;
			//std::cout << "Time is " << t << " s\n";
			float w = -250.0;
			std::complex<float> res(0.0, 0.0);
			for (int w_n = 1; w < 10001; w++) {
				w += 0.05;
				int ind = rec + (w_n - 1) * rec;
				res += data_w[ind] * std::exp(-std::complex<float>(0.0, 1.0) * w * t ) * -std::complex<float>(0.05 / 2.0 / M_PI, 0.0);
			}
			int ind = rec + (t_n - 1) * rec;
			data_t[ind] = res;
		}
	}
}

std::complex<float> SeismicDataBorn::calculateMatrixElement(float w, vector3 r_rec, vector3 r_o, vector3 r) {
	float coef = std::pow(w, 2.0) * d_x * d_z / 16.0 / M_PI / M_PI;
	float D = distance(r, r_o);
	if ((D < d_x) || (D < d_z))
		return std::complex<float>(0.0, 0.0);
	float d = distance(r, r_rec);
	if ((d < d_x) || (d < d_z))
		return std::complex<float>(0.0, 0.0);
	std::complex<float> arg = std::complex<float>(0.0, 1.0) * w / c_b * (D + d);
	return coef / D / d * std::exp(arg);
}

void SeismicDataBorn::saveModel(const char *filename) {
	std::ofstream ofs;
	ofs.open(filename, std::ofstream::out);
	if (ofs.fail())
		throw std::runtime_error("saveModel: Can't open file.");
	/* Make header. */
	ofs << "# vtk DataFile Version 3.0\n";
	ofs << "Created by AC_INVERSE\n";
	ofs << "ASCII\n";
	ofs << "DATASET STRUCTURED_POINTS\n";
	ofs << "DIMENSIONS " << n_x << " " << 1 << " " << n_z << "\n";
	ofs << "SPACING " << d_x << " " << 0 << " " << d_z << "\n";
	ofs << "ORIGIN 0 0 0\n";
	ofs << "POINT_DATA " << n_x * n_z << "\n";
	ofs << "SCALARS ds-sqr float 1\n";
	ofs << "LOOKUP_TABLE ds-sqr_table \n";
	/* Save model. */
	for (int k = 0; k < n_z; k++) {
		for (int i = 0; i < n_x; i++) {
			int ind = i + k * n_x;
			ofs << model[ind] << " ";
		}
		ofs << std::endl;
	}
	ofs.close();
}

void SeismicDataBorn::saveData(const char *filename) {
	std::ofstream ofs;
	ofs.open(filename, std::ofstream::out);
	if (ofs.fail())
		throw std::runtime_error("saveData: Can't open file.");
	/* Make header. */
	ofs << "# vtk DataFile Version 3.0\n";
	ofs << "Created by AC_INVERSE\n";
	ofs << "ASCII\n";
	ofs << "DATASET STRUCTURED_POINTS\n";
	ofs << "DIMENSIONS " << n_x << " " << 1 << " " << 1000 << "\n";
	ofs << "SPACING " << d_x << " " << 0 << " " << d_z << "\n";
	ofs << "ORIGIN 0 0 0\n";
	ofs << "POINT_DATA " << n_x * 1000 << "\n";
	ofs << "SCALARS p-t float 1\n";
	ofs << "LOOKUP_TABLE p-t_table \n";
	/* Save model. */
	// FIXME Do smth understandable here!
	for (int k = 0; k < 1000; k++) {
		for (int i = 0; i < n_x; i++) {
			int ind = i + k * n_x;
			ofs << data_t[ind].real() << " ";
		}
		ofs << std::endl;
	}
	ofs.close();
}
