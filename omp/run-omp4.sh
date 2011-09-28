#  "# "に"@"と"$"が続く行は、NQSオプションの指定
# @$-eo
#
# キューの指定
# @$-q sh20100
# @$-lP 1
# @$-lp 4
# @$-lm 1800mb
# @$-cp 0:10:00
set -x
OMP_NUM_THREADS=4
export OMP_NUM_THREADS

cd $QSUB_WORKDIR
./a.out
