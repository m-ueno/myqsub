# runomp
#set -x
PROGS=("enshu_omp" "enshu_omp_with_firsttouch" "enshu_omp_mpilike")
THREAD_NUMS=(1 2 4 8 12 16)
for prog in ${PROGS[@]}; do
    echo $prog
    for i in ${THREAD_NUMS[@]}; do
        OMP_NUM_THREADS=i
#        ./$prog
        echo $i $prog
    done
done
