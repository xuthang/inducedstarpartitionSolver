#!/bin/bash

LOC=../logs/measuring
mkdir -p $LOC


SMALL=${LOC}/small.csv
SOLVER_EXE=../exe/speedTest/speedTestRunner

calc(){ awk "BEGIN { print "$*" }"; }

if [ ! -f $SMALL ] ; then
	echo n,m,edge_rate,vc,q,time > $SMALL
elif [ $(cat $SMALL | head -n 1) != "n,m,edge_rate,vc,q,time" ] ; then
	cat $SMALL > $LOC/tmptmptmp.csv
	echo n,m,edge_rate,vc,q,time > $SMALL
	cat $LOC/tmptmptmp.csv >> $SMALL
	rm $LOC/tmptmptmp.csv
fi

for edgerate in {1..10} ; do 
	floatEdgeRate=$( calc $edgerate/10 ) 
	for vc in {1..9} ; do
		for n in {10..100..10} ; do
			echo $n $vc $floatEdgeRate
			$SOLVER_EXE $n $vc $floatEdgeRate 2>/dev/null >> $SMALL
		done
	done
done