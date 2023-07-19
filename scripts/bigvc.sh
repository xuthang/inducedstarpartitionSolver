#!/bin/bash

LOC=../logs/measuring
mkdir -p $LOC
SOLVER_EXE=../exe/speedTest/speedTestRunner
calc() { awk "BEGIN { print "$*" }"; }
tmpLocation=${LOC}/tmptmp.txt
SOLVED=0
CNT=0

FILE=${LOC}/bigVC.csv
timeout_limit=1200

for vc in {13..15}; do for edgerate in {6..8..2}; do for n in {70..150..20}; do

	floatEdgeRate=$(calc $edgerate/10)
	echo $CNT - $(date +%H:%M:%S) - $n $vc $edge $floatEdgeRate

	timeout --foreground $timeout_limit bash -c "$SOLVER_EXE $n $vc $floatEdgeRate 2>/dev/null > $tmpLocation"
	RET=$?

	if [ $RET -eq 124 ]; then
		killall $SOLVER_EXE
		echo timed out
		echo $(cat $tmpLocation), >>$FILE
	else
		cat $tmpLocation
		cat $tmpLocation >>$FILE
		SOLVED=$((SOLVED + 1))
	fi

	CNT=$((CNT + 1))
done; done; done

echo solved $SOLVED/$CNT

rm $tmpLocation
