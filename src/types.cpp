#include "types.h"


float distance (vector3 r1, vector3 r2) {
	float squared = (r1.x - r2.x) * (r1.x - r2.x) +
		(r1.y - r2.y) * (r1.y - r2.y) +
		(r1.z - r2.z) * (r1.z - r2.z);
	return sqrt(squared);
}
