# nqs_mpi_fujitsu.sh
#  "#"で始まる行はコメント。ただし、
#  "# "に"@"と"$"が続く行は、NQSオプションの指定
#========== NQSオプション ==========
NUM_PROCS=4
# @$-eo
#
# キューの指定
# @$-q sh20100
# @$-lP $NUM_PROCS
# @$-lp 1
# @$-lm 1800mb
# @$-cp 0:10:00

set -x
cd $QSUB_WORKDIR
mpiexec -n $NUM_PROCS a.out
mpiexec -n $NUM_PROCS a.out
