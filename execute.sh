#!/bin/bash

for i in {0..9..1}
do
	qsub -q $1 /home/ceskajak/FD/runs/run00$i/Bottom00$i.sh
done

for i in {10..99..1}
do
	qsub -q $1 /home/ceskajak/FD/runs/run0$i/Bottom0$i.sh
done

