#!/usr/bin/python
import math
print "Generating zero-offset seismogram 2 layered media. Source - Ricker."
# Ricker wavelet
A = 1000000.0
f = 50.0
# Model
c = 2000.0
l = 1000.0
# Numerical parameters
dt = 0.01
nt = 500
dx = dy = dz = 20
nx = 75
ny = nz = 75

fd = open("2layers0offset.dat", "w")
fd.write(
	"# N_STEPS\n# N_X\n# N_Y\n# N_Z\n# DT\n# H_X\n# H_Y\n# H_Z\n# P(i,j) \\tab P(i+1,j) ... \\tab P(i,j+1) \\tab P(i+1,j+1)\n"
	)

fd.write("{0}\n{1}\n{2}\n{3}\n{4}\n{5}\n{6}\n{7}\n".format(nt, nx, ny, nz, dt, dx, dy, dz));

for k in range(0, nt):
	for j in range(0, ny):
		for i in range(0, nx):
			d = 2.0 * l
			t = k * dt - d / c
			val = 1.0 / (4.0 * math.pi * d) * A * (1.0 - 2.0 * math.pi * math.pi * f * f * t * t) * math.exp(-math.pi * math.pi * f * f * t * t)
			fd.write("{0}\t".format(val))
	fd.write("\n")
fd.close()
