# run-mpi2
# @$-eo
# @$-q gh10034
# @$-g gh10034
# @$-lP 16
# @$-lp 1
# @$-lm 1800mb
# @$-cp 0:10:00

cd $QSUB_WORKDIR
NUM_PROCS=(2 4 8 12 16)
for i in ${NUM_PROCS[@]}; do
    echo $i procs
    count=10
    while [ $count -gt 0 ]; do
        mpiexec -band 16 -n $i ./a.out
        count=`expr $count - 1`
    done
done
