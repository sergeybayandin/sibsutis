#!/bin/bash
src="../src/timer.c ../src/csv.c ../src/test_cpu/cpuinfo.c ../src/test_cpu/test_cpu.c main.c"
args=$(echo $@ | uniq)
optimizations=$(echo $@ | grep -o "\-O[0-9]*")
if [[ -n $optimizations || ! -e "benchmark" ]]
then
	gcc -lm $optimizations -o benchmark $src && ./benchmark $args
else
	./benchmark $args
fi
