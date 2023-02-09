#!/bin/bash
for SIZE in 3840 5120 6400 7680 8960 
do
nvcc -DSIZE=$SIZE OffloadCUDA.cu -o cuda -Xcompiler --std=c++11
rm resultsDurationCuda/duration_$SIZE.txt
touch resultsDurationCuda/duration_$SIZE.txt
echo "Size $SIZE" >> resultsDurationCuda/duration_$SIZE.txt
ncu --metric sm__sass_thread_inst_executed_op_dfma_pred_on,sm__cycles_active.avg.per_second,sm__cycles_active ./cuda -c 5 >> resultsDurationCuda/duration_$SIZE.txt
done

 
