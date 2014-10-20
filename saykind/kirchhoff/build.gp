set terminal postscript enhanced color 
set output 'plot.eps'

set xlabel "X"
set ylabel "Z"
set zlabel "Pressure"
set xrange [-100:100]
set yrange [0:100]

splot 'ricker.dat' t 'data'
