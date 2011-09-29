#  "# "に"@"と"$"が続く行は、NQSオプションの指定
# @$-eo
# @$-q sh20100
# @$-lP 16
# @$-lp 1
# @$-lm 1800mb
# @$-cp 0:10:00

cd $QSUB_WORKDIR
NUM_CPUS=(1 2 4 8 12 16)
for i in ${NUM_CPUS[@]}; do
    count=10
    while [ $count -gt 0 ]; do
        mpiexec -n $i a.out
        count=`expr $count - 1`
    done
done

