#!/usr/bin/bash
mpicc -o task2 task2_.c
chmod +x s.sh
funcname=("matmulijk" "matmulSchleifenvertauschjki" "matmulijkTiling" "matmulijkWTemporary")
size=(128 256 512 1024 2048)
loop=(500 100 20 1 1)

for f in 0 1 2 3
do
nameing=${funcname[f]}
for b in 0 1 2 3 4
do
d=${size[b]}
l=${loop[b]}

echo $d>>output_${nameing}.txt
echo $d
for i in 1 2 3 4 5 6 7 8 9 10
do
./task2 $d $l $f>>output_${nameing}.txt
done
done
done