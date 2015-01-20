#!/usr/bin/bash
i=1
while [ $i -lt 1000 ]
do
    echo "$i">> exec4.out
    i=`expr $i + 1`
    sleep 1
done



