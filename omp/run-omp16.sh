#  "# "に"@"と"$"が続く行は、NQSオプションの指定
# @$-eo
#
# キューの指定
# @$-q sh20100
# @$-lP 1
# @$-lp 16
# @$-lm 1800mb
# @$-cp 0:10:00
set -x
OMP_NUM_THREADS=16
export OMP_NUM_THREADS

cd $QSUB_WORKDIR
./a.out
./a.out
./a.out
./a.out

