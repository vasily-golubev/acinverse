set terminal postscript enhanced color 
set output 'plot.eps'

set xlabel "X"
set ylabel "Z"
set zlabel "P"
set xrange [-110:110]
set yrange [0:150]

splot 'data.txt' w lines notitle
