# @$-eo
# @$-q gh10034
# @$-g gh10034
# @$-lP 64
# @$-lp 1
## @$-lm 1800mb
# @$-cp 0:10:00

set -x
cd $QSUB_WORKDIR
mpiexec -band 16 -n 64 ./b.out
