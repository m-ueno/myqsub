# @$-eo
#
# キューの指定
# @$-q sh20100
# @$-lP 1
# @$-lp 4
# @$-lm 1800mb
# @$-cp 0:10:00
set -x

cd $QSUB_WORKDIR
./a.out
./a.out
./a.out
./a.out
./a.out
./a.out
./a.out
./a.out
./a.out
./a.out
