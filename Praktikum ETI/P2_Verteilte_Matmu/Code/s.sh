#!/usr/bin/bash
module load OpenMPI/4.1.1
mpicc -o task2 task2_.c
chmod +x s.sh
funcname=("matmulijk" "matmulSchleifenvertauschjki" "matmulijkTiling" "matmulijkWTemporary")
size=(144 288 576 1152 2304)
loop=(1 1 1 1 1)

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
srun -t 00:20:00 --ntasks=12 --mem-per-cpu=1700 --cpus-per-task=2 -A p_lv_eti2223 --exclusive ./task2 $d $l $f>>output_${nameing}.txt
done
done
done