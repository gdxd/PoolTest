#!/bin/bash
#

if [ $# -lt 2 ]; then
    echo "$0 njobs testname"
    exit 1
fi

njobs=$1
testname=$2

runtime=180

rndseek=""
output=""
if [ x$3 == "x-r" ]; then
   rndseek="-r 1"
elif [ x$3 == "x-o" ]; then
   output="-o 1"
fi

#export DCTESTDIR=/root/dc_test
export DCTESTDIR=/root/PoolTest
#export DCTESTDIR=/home/g/GDuckeck/dCache/PoolTest

host=`hostname -s`

OUTDIR=$DCTESTDIR/$host.$testname.$njobs


# name of ile containing dcap file-lists
FILELIST=$DCTESTDIR/dctest.list

mkdir -p $OUTDIR

now=`date '+%s'`

(( start = now + 20 )) # reading starts in 200 secs
(( end = start + runtime )) # run for 'runtime' secs



for i in `seq 1 $njobs`; do
    echo 3 > /proc/sys/vm/drop_caches # clear fs cache
    logfile=$OUTDIR/dctest.out.$i
    python3 $DCTESTDIR/dctest3.py $rndseek $output -s $start -e $end -l $FILELIST > $logfile &
done

(( wt = 40 + runtime )) 

sleep $wt

