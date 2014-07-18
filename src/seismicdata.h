#ifndef ACINVERSE_SEISMIC_DATA_H
#define ACINVERSE_SEISMIC_DATA_H

#include <fstream>

class SeismicData {
	public:
		SeismicData();
		~SeismicData();
		/*void setDimensions(int n_x, int n_y, int n_t);*/
		void setDimensions(std::ifstream *ifs);
		void setDXYZT(std::ifstream *ifs);
		void readData(std::ifstream*);
		void printDimensions();
		void print();
		void skipHeader(std::ifstream *ifs);
		float getValue(float x, float y, float t);
		void saveImage();
		void saveSeismograms(const char *name);
		void setFilename(const char *filename) { this->filename = filename; };
		void generatePointSource(float x, float y, float z, float c);
		void generatePointRickerWavelet(float x, float y, float z, float c, float f);
		float dX() { return d_x; };
		float dY() { return d_y; };
		float dZ() { return d_z; };
		float nX() { return n_x; };
		float nY() { return n_y; };
		float nZ() { return n_z; };
		float *image;
	private:
		int n_x;
		int n_y;
		int n_z;
		int n_t;
		float d_x;
		float d_y;
		float d_z;
		float d_t;
		float *data;
		const char *filename;
};

#endif
