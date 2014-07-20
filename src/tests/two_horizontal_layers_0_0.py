#!/usr/bin/python
import math
print "Generating config for 2 layered media with Ricker point source at 0,0."
# Ricker wavelet.
A = 1000000.0
f = 50.0
# Model parameters.
c = 2000.0
l = 1000.0
# Discretization.
dt = 0.001
nt = 3000
dx = dy = dz = 20.0
nx = 1
ny = nz = 100

fd = open("two_horizontal_layers_0_0.dat", "w")
fd.write(
	"# N_STEPS\n# N_X\n# N_Y\n# N_Z\n# DT\n# H_X\n# H_Y\n# H_Z\n# P(i,j) \\tab P(i+1,j) ... \\tab P(i,j+1) \\tab P(i+1,j+1)\n"
	)

fd.write("{0}\n{1}\n{2}\n{3}\n{4}\n{5}\n{6}\n{7}\n".format(nt, nx, ny, nz, dt, dx, dy, dz));

for k in range(0, nt):
	for j in range(0, ny):
		for i in range(0, nx):
			# FIXME Only for 2D case is valid!
			d = math.sqrt(4.0 * l * l + (j * dy) * (j * dy))
			t = k * dt - d / c
			val = 1.0 / (4.0 * math.pi * d) * A * (1.0 - 2.0 * math.pi * math.pi * f * f * t * t) * math.exp(-math.pi * math.pi * f * f * t * t)
			if (j > 0):
				s_t = k * dt - j * dy / c
				s_val = 1.0 / (4.0 * math.pi * j * dy) * A * (1.0 - 2.0 * math.pi * math.pi * f * f * s_t * s_t) * math.exp(-math.pi * math.pi * f * f * s_t * s_t)
				val += s_val
			fd.write("{0}\t".format(val))
	fd.write("\n")
fd.close()
