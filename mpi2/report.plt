set term postscript eps enhanced
set output "fig.eps"
set xlabel "x"
set ylabel "y"
set view 120, 320
sp 'u.data' w pm3d

