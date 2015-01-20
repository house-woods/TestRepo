#!/usr/bin/bash
#while read lines
#do
#    echo "press any key"
#    read key
#done
#for line in `cat file`
#do
#    echo ${line}
#    echo "press any key"
#    read key
#done

#while read line
#do
#    echo ${line}
#    read key
#done <file
cat file | while read line
do
    echo $line
    (echo "preass any key:c";exec </dev/tty;read key)
done



