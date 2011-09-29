# nqs_mpi_fujitsu.sh
# @$-eo
#
# キューの指定
# @$-q gh10034
# @$-g gh10034
# @$-lP 1
# @$-lp 16
# @$-lm 1800mb
# @$-cp 0:10:00

set -x # for debug
OMP_NUM_THREADS=16
cd $QSUB_WORKDIR
./a.out
