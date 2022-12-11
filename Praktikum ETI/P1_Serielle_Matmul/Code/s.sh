#!/bin/bash
size=(100 100 100 100 100)
loop=(100 100 100 100 100)
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
