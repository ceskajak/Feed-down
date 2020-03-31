#!/bin/bash

mkdir runs
cd runs

echo run folder generated

for i in {0..9..1}
do
	mkdir "run00$i"
	cp ../src/Bottom.cmnd run00$i
	cp ../src/Bottom.cc run00$i
	cp ../src/Bottom.sh run00$i/Bottom00$i.sh
	cd run00$i
	sed -i "s/Bottom.root/Bottom00$i.root/g" Bottom.cc
	sed -i "s/error/error00$i/g" Bottom00$i.sh
	sed -i "s/RUN_no/run00$i/g" Bottom00$i.sh
	/home/ceskajak/scripts/compile.sh
	cd ..
	
	echo run00$i generated
done

for i in {10..99..1}
do
	mkdir "run0$i"
	cp ../src/Bottom.cmnd run0$i
	cp ../src/Bottom.cc run0$i
	cp ../src/Bottom.sh run0$i/Bottom0$i.sh
	cd run0$i
	sed -i "s/Bottom.root/Bottom0$i.root/g" Bottom.cc
	sed -i "s/error/error0$i/g" Bottom0$i.sh
	sed -i "s/RUN_no/run0$i/g" Bottom0$i.sh
	/home/ceskajak/scripts/compile.sh
	cd ..
	
	echo run0$i generated
done
