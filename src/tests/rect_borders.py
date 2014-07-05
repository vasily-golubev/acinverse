#!/usr/bin/python
print "Generating config for media with rect borders."
c = 2000
dt = 0.01
nt = 230
# width of impulse 400
dx = dy = dz = 20
ny = 1
nx = nz = 75

fd = open("rect_borders.dat", "w")
fd.write(
	"# N_STEPS\n# N_X\n# N_Y\n# N_Z\n# DT\n# H_X\n# H_Y\n# H_Z\n# P(i,j) \\tab P(i+1,j) ... \\tab P(i,j+1) \\tab P(i+1,j+1)\n"
	)

fd.write("230\n75\n1\n75\n0.01\n20\n20\n20\n");

# Before response.
for t in range(0, 50):
	for j in range(0, ny):
		for i in range(0, nx):
			fd.write("0.0\t")
	fd.write("\n")
for t in range(50, 70):
	for j in range(0, ny):
		for i in range(0, nx):
			if (i < nx / 3):
				fd.write("1.0\t")
			else:
				fd.write("0.0\t")
	fd.write("\n")
for t in range(70, 100):
	for j in range(0, ny):
		for i in range(0, nx):
			fd.write("0.0\t")
	fd.write("\n")
for t in range(100, 120):
	for j in range(0, ny):
		for i in range(0, nx):
			if (i > nx / 3):
				fd.write("1.0\t")
			else:
				fd.write("0.0\t")
	fd.write("\n")
for t in range(120, nt):
	for j in range(0, ny):
		for i in range(0, nx):
			fd.write("0.0\t")
	fd.write("\n")
fd.close()
