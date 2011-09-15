# nqs_mpi_fujitsu.sh
#  "#"で始まる行はコメント。ただし、
#  "# "に"@"と"$"が続く行は、NQSオプションの指定
#========== NQSオプション ==========
# @$-eo
#
# キューの指定
# @$-q sh20100
# @$-lP 1
# @$-lp 16
# @$-lm 1800mb
# @$-cp 0:10:00

set -x

#----------- 環境変数の指定 ------------
#
# スタックサイズの情報を取得(Fat SMPクラスタのFortranのみ有効)
# FLIB_USE_STACK_INFO=1;export FLIB_USE_STACK_INFO

#---------- プログラムの実行 ----------
# ジョブを投入したディレクトリに移動
cd $QSUB_WORKDIR
time ./a.out

