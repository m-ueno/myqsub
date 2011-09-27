set term postscript eps enhanced
set output "fig.eps"
set xlabel "x"
set ylabel "y"
sp 'u.data' w pm3d

