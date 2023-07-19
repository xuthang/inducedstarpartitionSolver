#!/bin/bash

LOC=../logs/measuring
mkdir -p $LOC

SOLVER_EXE=../exe/speedTest/speedTestRunner

calc(){ awk "BEGIN { print "$*" }"; }

timeout_limit=1200
tmpLocation=${LOC}/tmptmp.txt
SOLVED=0
CNT=0

GIGANTIC=${LOC}/bign.csv
for vc in {6..8} ; do for edgerate in {6..6..2} ; do for n in {2000..10000..250} ; do
	floatEdgeRate=$( calc $edgerate/10 ) 
	echo $CNT - $(date +%H:%M:%S) - $n $vc $edge $floatEdgeRate

	timeout --foreground $timeout_limit bash -c "$SOLVER_EXE $n $vc $floatEdgeRate 2>/dev/null > $tmpLocation"
	RET=$?

	if [ $RET -eq 124 ] ; then
		killall $SOLVER_EXE
		echo timed out
		echo $(cat $tmpLocation), >> $GIGANTIC
	else
		cat $tmpLocation
		cat $tmpLocation >> $GIGANTIC
		SOLVED=$((SOLVED+1))
	fi

	CNT=$((CNT+1))
done done done

rm $tmpLocation