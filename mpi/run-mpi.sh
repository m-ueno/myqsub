#  "# "に"@"と"$"が続く行は、NQSオプションの指定
# @$-eo
# @$-q sh20100
# @$-lp 1
# @$-lm 1800mb
# @$-cp 0:10:00
# @$-o ret-mpi.txt

cd $QSUB_WORKDIR
NUM_CPUS=(1 2 4 8 12 16)
for i in ${NUM_CPUS[@]}; do
# @$-lP $i
    for temp in 1..10; do
        echo mpiexec -n $i a.out
    done
done

