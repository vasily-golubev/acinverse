#!/usr/bin/python
print "Generating cofig for point source usage."
c = 2500
dt = 0.001
nt = 5000

dx = dy = dz = 20
nx = 1
ny = nz = 200

fd = open("point_source.dat", "w")
fd.write(
	"# N_STEPS\n# N_X\n# N_Y\n# N_Z\n# DT\n# H_X\n# H_Y\n# H_Z\n# P(i,j) \\tab P(i+1,j) ... \\tab P(i,j+1) \\tab P(i+1,j+1)\n"
	)

fd.write("{0}\n{1}\n{2}\n{3}\n{4}\n{5}\n{6}\n{7}\n".format(nt, nx, ny, nz, dt, dx, dy, dz));

for t in range(0, nt):
	for j in range(0, ny):
		for i in range(0, nx):
			fd.write("0.0\t")
	fd.write("\n")
fd.close()
