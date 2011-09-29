# @$-eo
# @$-q gh10034
# @$-g gh10034
# @$-lP 16
# @$-lp 1
# @$-lm 1800mb
# @$-cp 0:10:00

cd $QSUB_WORKDIR
echo $QSUB_WORKDIR
#cd /home/a/a0064164/work/EnshuB/mpi

NUM_CPUS=(2 4 8 12 16)
for i in ${NUM_CPUS[@]}; do
    count=10
    while [ $count -gt 0 ]; do
        mpiexec -n $i ./a.out
        count=`expr $count - 1`
    done
done
