#ifndef ACINVERSE_SEISMIC_DATA_H
#define ACINVERSE_SEISMIC_DATA_H

#include <fstream>

#define MAX_SIZE 200

class SeismicData {
	public:
		SeismicData();
		void setDimensions(int n_x, int n_y, int n_t);
		void setDimensions(std::ifstream *ifs);
		void setDXYZT(std::ifstream *ifs);
		void readData(std::ifstream*);
		void printDimensions();
		void print();
		void skipHeader(std::ifstream *ifs);
		float getValue(float x, float y, float t);
		void saveImage();
		void setFilename(const char *filename) { this->filename = filename; };
		float dX() { return d_x; };
		float dY() { return d_y; };
		float dZ() { return d_z; };
		float nX() { return n_x; };
		float nY() { return n_y; };
		float nZ() { return n_z; };
		float image[MAX_SIZE][MAX_SIZE][MAX_SIZE];
	private:
		int n_x;
		int n_y;
		int n_z;
		int n_t;
		float d_x;
		float d_y;
		float d_z;
		float d_t;
		/* data[along X][along Y][along T] */
		float data[MAX_SIZE][MAX_SIZE][MAX_SIZE];
		const char *filename;
};

#endif
