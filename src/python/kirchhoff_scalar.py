#!/usr/bin/python

from math import exp, sqrt, pi

print '''Solver of direct problem for scalar wave equation \
based on Kirchhoff formula'''

# My definitions: rr = r', tt = t'

# Mesh parameters
nx = 100
ny = 1
nz = 50
dx = 10.0
dy = 20.0
dz = 20.0
data = [0.0 for ind in range(nx * ny * nz)]

# Medium properties
c = 2000.0

# The case of Ricker P-wave with frequency f
f = 50.0
amp = 1.0

# FIXME
dt = 0.01

class Vector():
	x = 0
	y = 0
	z = 0

	def __init__(self, x = 0, y = 0, z = 0):
		self.x = x
		self.y = y
		self.z = z

	def __sub__(self, other):
		return Vector(self.x - other.x,
				self.y - other.y,
				self.z - other.z)

def getDistance(r, rr):
	d = Vector()
	d = r - rr
	res = sqrt(d.x * d.x + d.y * d.y + d.z * d.z)
	return res

def initProblem():
	pass

def getSurfacePressure(r, rr, tt):
	t = tt - getDistance(r, rr) / c
	return amp * (1 - 2.0 * pi * pi * f * f * t * t) * exp(-pi * pi * f * f * t * t)

def getDTTSurfacePressure(r, rr, tt):
	return (getSurfacePressure(r, rr, tt + dt) - getSurfacePressure(r, rr, tt)) / dt

def getDZSurfacePressure(r, rr, tt):
	r_dz = Vector(r.x, r.y, r.z + dz)
	return (getSurfacePressure(r_dz, rr, tt + dt) - getSurfacePressure(r, rr, tt)) / dz

def getSingleValue(r, rr, tt):
	d = getDistance(r, rr)
	res = 1.0 / c * getDTTSurfacePressure(r, rr, tt) + 1.0 / d / d * getSurfacePressure(r, rr, tt)
	res *= (rr.z - r.z) / d
	res -= 1.0 / d * getDZSurfacePressure(r, rr, tt)
	return res

def getDKSISurfacePressure(r, rr, tt):
	d = getDistance(r, rr)
	t = tt - d
	return -2.0 * pi * pi * f * f * amp * t * (3.0 - 2.0 * pi * pi * f * f * t * t) * exp(-pi * pi * f * f * t * t)

def getDTTSurfacePressureAnalytic(r, rr, tt):
	return getDKSISurfacePressure(r, rr, tt)

def getDZSurfacePressureAnalytic(r, rr, tt):
	d = getDistance(r, rr)
	return getDKSISurfacePressure(r, rr, tt) * (rr.z - r.z) / d / c

def getSingleValueAnalytic(r, rr, tt):
	d = getDistance(r, rr)
	res = 1.0 / c * getDTTSurfacePressureAnalytic(r, rr, tt) + 1.0 / d / d * getSurfacePressure(r, rr, tt)
	res *= (rr.z - r.z) / d
	res -= 1.0 / d * getDZSurfacePressureAnalytic(r, rr, tt)
	return res

def saveImageVTK(tt):
	f = open("screenshot_{0}.vtk".format(tt), "w")
	# Write header
	f.write("# vtk DataFile Version 3.0\n")
	f.write("Created by AC_INVERSE\n")
	f.write("ASCII\n")
	f.write("DATASET STRUCTURED_POINTS\n")
        f.write("DIMENSIONS {0} {1} {2}\n".format(nx, ny, nz))
	f.write("SPACING {0} {1} {2}\n".format(dx, dy, dz))
	f.write("ORIGIN 0 0 0\n")
	f.write("POINT_DATA {0}\n".format(nx * ny * nz))
	f.write("SCALARS P float 1\n")
	f.write("LOOKUP_TABLE P_table \n")
	# Write data
	for k in range(nz):
		for j in range(ny):
			for i in range(nx):
				ind = i + j * nx + k * nx * ny
				f.write(str(data[ind]) + " ")
		f.write("\n")
	f.close()

def initProblem():
	pass

# Here is the entrance point in the program

initProblem()
r = Vector()
rr = Vector()

for tt in (0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8):
	for kk in range(1, nz):
		for jj in range(ny):
			# FIXME rr == r => BANG!
			for ii in range(nx):
				P = 0.0
				rr.x = ii * dx
				rr.y = jj * dy
				rr.z = kk * dz
				for j in range(ny):
					for i in range(nx):
						r.x = i * dx
						r.y = j * dy
						r.z = 0
						P += 1.0 / (4.0 * pi) * getSingleValueAnalytic(r, rr, tt) * dx * dy
				ind = ii + jj * nx + kk * nx * ny
				data[ind] = P
		# TODO Some interactive tool
		print "Z-layer:", kk

	# Save screenshot at the final stage
	saveImageVTK(tt)
