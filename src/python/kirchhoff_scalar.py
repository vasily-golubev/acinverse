#!/usr/bin/python

from math import sqrt, pi

print '''Solver of direct problem for scalar wave equation \
based on Kirchhoff formula'''

# My definitions: rr = r', tt = t'

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

c = 2000.0
dt = 1.0
dx = 1.0
dy = 1.0
nx = 100
ny = 100

def getDistance(r, rr):
	d = Vector()
	d = r - rr
	res = sqrt(d.x * d.x + d.y * d.y + d.z * d.z)
	return res

def initProblem():
	pass

def getSurfacePressure(r, rr, tt):
	# The case of P-wave with length L m
	L = 100.0
	P_0 = 1.0
	t = tt - getDistance(r, rr) / c
	if (t > 0.0) and (t < L / c):
		return P_0
	else:
		return 0.0

def getDTTSurfacePressure(r, rr, tt):
	# FIXME What about dt value?
	dt = 0.1
	return (getSurfacePressure(r, rr, tt + dt) - getSurfacePressure(r, rr, tt)) / dt

def getDZSurfacePressure(r, rr, tt):
	# FIXME What about dz value?
	dz = 0.1
	r_dz = Vector(r.x, r.y, r.z + dz)
	return (getSurfacePressure(r_dz, rr, tt + dt) - getSurfacePressure(r, rr, tt)) / dt

def getSingleValue(r, rr, tt):
	d = getDistance(r, rr)
	res = 1.0 / c * getDTTSurfacePressure(r, rr, tt) + 1.0 / d / d * getSurfacePressure(r, rr, tt)
	res *= (rr.z - r.z) / d
	res -= 1.0 / d * getDZSurfacePressure(r, rr, tt)
	return res

# Here is the entrance point in the program

initProblem()
r = Vector(1.0, 2.0, 3.0)
rr = Vector(400.0, 5.0, 6.0)
tt = 0.0
# FIXME Calculate for whole mesh
P = 0.0
for j in range(ny):
	for i in range(nx):
		r.x = i * dx
		r.y = j * dy
		r.z = 0
		P += 1.0 / (4.0 * pi) * getSingleValue(r, rr, tt) * dx * dy

print "Pressure is equal to", P
