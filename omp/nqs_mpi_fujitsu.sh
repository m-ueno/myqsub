# nqs_mpi_fujitsu.sh
#
# 富士通コンパイラ+プロセス並列用(MPI, XPFortran)
#
#  "#"で始まる行はコメント。ただし、
#  "# "に"@"と"$"が続く行は、NQSオプションの指定
#
#========== NQSオプション ==========
#
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

