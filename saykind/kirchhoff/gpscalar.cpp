#include <iostream>
#include <cmath>	//sqrt
#include <cstdlib>	//atof
#include <cstdio>	//fprintf
#include <unistd.h>	//usleep

#define nl '\n'
#define pi M_PI

#define Ax -100.0
#define Ay -100.0
#define Az  0.0
#define Bx  100.0
#define By  100.0
#define Bz  0.0
#define H   200.0
#define Nx  32
#define Ny  32
#define Nz  64
#define Nt  20

#define P0 1
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
	return S;
}

FILE *gpinit(void) {	
	FILE *gp = popen("gnuplot","w");
	fprintf(gp, "set xlabel 'X'\n set ylabel 'Z'\n set zlabel 'P'\n");
	fprintf(gp, "set xrange [%.0f:%.0f]\n set yrange [%.0f:%.0f]\n", Ax, Bx, 0.0, H);
	return gp;
}	

void splot(FILE *gp, double t, double data[Nx][Nz]) {
	fprintf(gp, "splot '-' t 't = %.4lf'\n", t);
	int ix, iz;
	for (ix = 0; ix < Nx; ix++) {
		for (iz = 0; iz < Nz; iz++) {
			fprintf(gp, "%lf %lf %lf \n", 
			Ax + dx*ix, Az + dz*iz, data[ix][iz]);
		}
	}
	fprintf(gp, "e\n");
	fflush(gp);
}
int main(int argv, char *argc[]) {
	if (argv < 2) {
		cout << "One arguments (time until) is requied\n"; 
		return 0;
	}
	double t = atof(argc[1]);

	FILE *gp = gpinit();
	
	double data[Nx][Nz];
	int ix, iy, iz, it;
	for (ix = 0; ix < Nx; ix++) 
		for (iz = 0; iz < Nz; iz++) 
			data[ix][iz] = 0.0;
	splot(gp, t, data); 
	vector R;
	double dt = t/Nt;
	for (it = 0; it < Nt; it++) {
		t = dt*it;
		for (ix = 0; ix < Nx; ix++) {
			for (iz = 0; iz < Nz; iz++) {
				vector r(Ax + dx*ix, .0, Az + dz*iz);
				if (r == R) {continue;}
				data[ix][iz] = integrate(t, r, R);
			}
		}
		splot(gp, t, data); 
	}
	sleep(10);
	pclose(gp);
	return 0;
}

// r' = R (source)
// r  = r (point)
