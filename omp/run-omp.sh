# run-omp
# @$-eo
# @$-q sh20100
# @$-lP 1
# @$-lp 16
# @$-lm 1800mb
# @$-cp 0:10:00

PROGS=("enshu_omp" "enshu_omp_with_firsttouch" "enshu_omp_mpilike")
NUM_THREADS=(1 2 4 8 12 16)
for prog in ${PROGS[@]}; do
    echo $prog
    for i in ${NUM_THREADS[@]}; do
        OMP_NUM_THREADS=i
        ./$prog
    done
done
