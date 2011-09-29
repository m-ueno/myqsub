# run-omp
# @$-eo
# @$-q sh20100
# @$-lP 1
# @$-lp 16
# @$-lm 1800mb
# @$-cp 0:10:00

cd $QSUB_WORKDIR

#set -x
NUM_THREADS=(1 2 4 8 12 16)
for i in ${NUM_THREADS[@]}; do
    OMP_NUM_THREADS=i
    ./enshu_omp
done
echo "----"
for i in ${NUM_THREADS[@]}; do
    OMP_NUM_THREADS=i
    ./enshu_omp_with_firsttouch
done
echo "----"
for i in ${NUM_THREADS[@]}; do
    OMP_NUM_THREADS=i
    ./enshu_omp_mpilike
done
