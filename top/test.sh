#!/bin/ksh
cnt=10
i=1
while [ $i -le $cnt ]
do
  echo $i 
  i="`expr $i + 1`"
  top -s 1 >>top.log
done
