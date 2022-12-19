#!/bin/bash
for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24                                                                        
do
srun --ntasks=2*${threads} --tasks-per-node=1024 --nodes=${node} --exclusive ./task2 >> output/output_${node}_${threads}.txt
done

salloc -t 00:20:00 --nodes=1 --tasks-per-node=48 --mem-per-cpu=1024 -p haswell --exclusive
marie@login$ srun --pty --ntasks=1 --cpus-per-task=4 --time=1:00:00 --mem-per-cpu=1700 bash -l
srun: job 13598400 queued and waiting for resources
srun: job 13598400 has been allocated resources
marie@compute$ # Now, you can start interactive work with e.g. 4 cores

srun --ntasks=4  --mincpus=2  --exclusive ./task2




mpicc -O3 -march=native -o task2 task2_.c 



ssh s7393169@taurus.hrsk.tu-dresden.de


