set term postscript eps enhanced color
set output "fig.eps"
set xlabel "x"
set ylabel "y"
set view 120, 320
sp 'u.data' w pm3d

