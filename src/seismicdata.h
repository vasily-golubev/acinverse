#include <fstream>

#define MAX_SIZE 100

class SeismicData {
	public:
		SeismicData();
		void setDimensions(int n_x, int n_y, int n_t);
		void setDimensions(std::ifstream *ifs);
		void setDXYT(std::ifstream *ifs);
		void readData(std::ifstream*);
		void printDimensions();
		void print();
		void skipHeader(std::ifstream *ifs);
		float getValue(float x, float y, float t);
	private:
		int n_x;
		int n_y;
		int n_t;
		float d_x;
		float d_y;
		float d_t;
		/* data[along X][along Y][along T] */
		float data[MAX_SIZE][MAX_SIZE][MAX_SIZE];
};
