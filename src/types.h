#ifndef ACINVERSE_TYPES_H
#define ACINVERSE_TYPES_H

#include <math.h>

#define IS_FLOAT_EQUAL(a, b) (fabs(a - b) < 1e-4) ? 1 : 0

struct vector2 {
	float x;
	float y;
	vector2(float px, float py) {
		x = px;
		y = py;
	}
};

struct vector3 {
	float x;
	float y;
	float z;
	vector3(float px, float py, float pz) {
		x = px;
		y = py;
		z = pz;
	}
};

typedef struct vector2 vector2;
typedef struct vector3 vector3;

float distance(vector3 r1, vector3 r2);

#endif
