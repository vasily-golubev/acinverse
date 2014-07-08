#!/usr/bin/python
print "Generating config for 2 layered media."
c = 2000
dt = 0.01
nt = 230
# width of impulse 400
width = 400.0
l = 1000.0
dx = dy = dz = 20
nx = 1
ny = nz = 75

fd = open("two_horizontal_layers.dat", "w")
fd.write(
	"# N_STEPS\n# N_X\n# N_Y\n# N_Z\n# DT\n# H_X\n# H_Y\n# H_Z\n# P(i,j) \\tab P(i+1,j) ... \\tab P(i,j+1) \\tab P(i+1,j+1)\n"
	)

fd.write("{0}\n{1}\n{2}\n{3}\n{4}\n{5}\n{6}\n{7}\n".format(nt, nx, ny, nz, dt, dx, dy, dz));

# Before response.
for t in range(0, int(2 * l / c / dt)):
	for j in range(0, ny):
		for i in range(0, nx):
			fd.write("0.0\t")
	fd.write("\n")
# Response.
for t in range(int(2 * l / c / dt), int (2 * l / c / dt + width / c / dt)):
	for j in range(0, ny):
		for i in range(0, nx):
			fd.write("1.0\t")
	fd.write("\n")
# After response.
for t in range(int(2 * l / c / dt + width / c / dt), nt):
	for j in range(0, ny):
		for i in range(0, nx):
			fd.write("0.0\t")
	fd.write("\n")
fd.close()
