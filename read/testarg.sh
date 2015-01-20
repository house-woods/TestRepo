#!/usr/bin/bash
#echo $0
#echo $*
#echo $@

trap "" 2 3 4 5 6 7 8 10 12 13 14 15
. ./testfile.sh&
/usr/bin/pwait $$

echo $$
while true
do
    sleep 1
done



ARG1="hello1"
ARG2="hello"
[ "X${ARG1}" = "X${ARG2}" ] && echo "same"



