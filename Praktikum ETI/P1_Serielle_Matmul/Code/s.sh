#!/usr/bin/bash
gcc -o task1 task1_.c
mpicc
mpicc -O3 -march=native  -o task2_ task2_.c
chmod +x s.sh
size=(128 256 512 1024 2048)
loop=(500 100 20 1 1)
for b in 0 1 2 3 4
do
d=${size[b]}
l=${loop[b]}
echo $d>>output.txt
echo $d
for i in 1 2 3 4 5 6 7 8 9 10
do

./task1 $d $l>>output.txt
done
done
--mem-per-cpu
-t 00:20:00 --ntasks=${threads} --mem-per-cpu=128 --cpus-per-task=${cpus} --exclusive ./task2 >> output/output_${cpus}_${threads}.txt

