#!/bin/bash
for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
do
echo "$threads"
srun --ntasks=$threads --tasks-per-node=$threads --exclusive ./task2 >> outputN1/output_${node}_${threads}.txt
done

