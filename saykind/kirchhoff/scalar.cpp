#include <iostream>
#include <cmath>	//sqrt
#include <cstdlib>	//atof
#include <cstdio>	//fprintf

#define nl '\n'
#define pi M_PI

#define Ax -100.0
#define Ay -100.0
#define Az  0.0
#define Bx  100.0
#define By  100.0
#define Bz  0.0
#define H   200.0
#define Nx  64
#define Ny  64
#define Nz  100

#define P0 1e6
#define f 50.0
#define C 200


double dx = (Bx-Ax)/Nx;
double dy = (By-Ay)/Ny;
double dz = H/Nz;
double ds = dx*dy;

using namespace std;

class vector {
	public:
	double x;
	double y;
	double z;
	vector() : x(.0), y(.0), z(.0) {};
	vector(double X) : x(X), y(.0), z(.0) {};
	vector(double X, double Y) : x(X), y(Y), z(.0) {};
	vector(double X, double Y, double Z) : x(X), y(Y), z(Z) {};
	double abs() {return sqrt(x*x +y*y + z*z);}
	double scl() {return x*x +y*y + z*z;}
	friend vector operator+(const vector a, const vector b);
	friend vector operator-(const vector a, const vector b);
	bool operator== (const vector a) const; 
};
inline vector operator+(const vector a, const vector b) {return vector(a.x + b.x, a.y + b.y, a.z + b.z);};
inline vector operator-(const vector a, const vector b) {return vector(a.x - b.x, a.y - b.y, a.z - b.z);};
inline bool vector::operator== (const vector a) const {return (x == a.x) && (y == a.y) && (z == a.z);}

double integrand(double t, vector r, vector R) {
	double DtP = -2.0*pi*pi*f*f*P0/C*(3.0 - 2.0*pi*pi*f*f*(t-(r-R).abs()/C)*(t-(r-R).abs()/C) )*
	         exp(-pi*pi*f*f*(t-(r-R).abs()/C)*(t-(r-R).abs()/C) );
	double P = P0*(1.0 - 2.0*pi*pi*f*f*(t-(r-R).abs()/C)*(t-(r-R).abs()/C) ) *
                 exp(-pi*pi*f*f*(t-(r-R).abs()/C)*(t-(r-R).abs()/C) )/(r-R).scl(); 
	double DzP = 2*pi*pi*f*f*P0*(t-(r-R).abs()) 
		* (3.0-2.0*pi*pi*f*f*(t-(r-R).abs()/C)*(t-(r-R).abs()/C)) 
		* exp( -pi*pi*f*f*(t-(r-R).abs()/C)*(t-(r-R).abs()/C) )*(r.z-R.z)/C/(r-R).abs();
//	printf("(%.2lf, %.2lf, %.2lf)\t", r.x, r.y, r.z);
//	cout << "DtP = " << DtP << "\tDzP = " << DzP << "\tP = " << P << nl; 
	return (1.0/4.0/pi)*( (r.z-R.z)/(r-R).abs() * (DtP/C + P/(r-R).scl()) - DzP/(r-R).abs() );
}

double integrate(double t, vector r, vector R) {
	double S  = .0;
	for (int ix = 0; ix < Nx; ix++)
		for (int iy = 0; iy < Ny; iy++) {
			S += ds*integrand(t, r, R);
		}
	cout << S;
	return S;
}
	 

int main(int argv, char *argc[]) {
	if (argv < 3) {
		cout << "Two arguments (time, outfile name) are requied\n"; 
		return 0;
	}
	double t = atof(argc[1]);
	FILE *fp = fopen(argc[2],"w");	
	vector R;
	fprintf(fp,"#t = %lf\n#x\tz\tP(x,z)\n", t);
	int ix, iy, iz;
	for (ix = 0; ix < Nx; ix++) {
		for (iz = 0; iz < Nz; iz++) {
			vector r(Ax + dx*ix, .0, dz*iz);
			if (r == R) {continue;}
			fprintf(fp, "%.2lf\t%.2lf\t%lf\n", Ax + dx*ix, dz*iz, integrate(t, r, R)); 
		}
	}
	return 0;
}

// r' = R (source)
// r  = r (point)
