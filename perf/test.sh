#!/bin/ksh
#set -x
LOG=test.log
num=0
while [ "$num" -lt 2 ]
do
  echo "**************************************************************" >>${LOG}
  echo "The ${num} sample ......" >>${LOG}
  date >>${LOG}
  sleep 10
  date >>${LOG}
  num=`expr $num + 1`
done

