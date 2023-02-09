#!/bin/bash
for SIZE in 3840 5120 6400 7680 8960 
do
nvc -O3 -DSIZE=$SIZE -mp=gpu openMPOffload_onlyMatmul.c -o offload
rm resultsDuration/duration_$SIZE.txt
touch resultsDuration/duration_$SIZE.txt
echo "Size $SIZE" >> resultsDuration/duration_$SIZE.txt
ncu --metric sm__sass_thread_inst_executed_op_dfma_pred_on,sm__cycles_active.avg.per_second,sm__cycles_active ./offload -c 5 >> resultsDuration/duration_$SIZE.txt
done

 
