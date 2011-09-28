#  "# "に"@"と"$"が続く行は、NQSオプションの指定
# @$-eo
#
# @$-q sh20100
# @$-lP 4
# @$-lp 1
# @$-lm 1800mb
# @$-cp 0:10:00

set -x
cd $QSUB_WORKDIR
NUM_PROCS=4
mpiexec -n $NUM_PROCS a.out
mpiexec -n $NUM_PROCS a.out
mpiexec -n $NUM_PROCS a.out
mpiexec -n $NUM_PROCS a.out
mpiexec -n $NUM_PROCS a.out
mpiexec -n $NUM_PROCS a.out
mpiexec -n $NUM_PROCS a.out
mpiexec -n $NUM_PROCS a.out
mpiexec -n $NUM_PROCS a.out
mpiexec -n $NUM_PROCS a.out
