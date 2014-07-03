#include <fstream>

class SeismicData {
	public:
		SeismicData() {n_x = n_y = n_t = 0;};
		void setDimensions(int n_x, int n_y, int n_t);
		void setDimensions(std::ifstream *ifs);
		void printDimensions();
		void skipHeader(std::ifstream *ifs);
	private:
		int n_x;
		int n_y;
		int n_t;
};
