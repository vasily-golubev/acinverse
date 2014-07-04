#!/bin/bash
g++ kirchhoff_migration.cpp \
	seismicdata.cpp \
	rayleighintegral.cpp \
	types.cpp \
	-Wall -g -o kirchhoff_migration -lm
