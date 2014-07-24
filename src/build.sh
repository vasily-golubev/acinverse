#!/bin/bash
g++ kirchhoff_migration.cpp \
	seismicdata.cpp \
	seismicdataborn.cpp \
	rayleighintegral.cpp \
	types.cpp \
	-Wall -g -o kirchhoff_migration -lm
