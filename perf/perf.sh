#!/bin/ksh
#set -x
LOG=perf.log
num=0 
while [ "$num" -lt 6 ] 
do
  echo "**************************************************************" >>${LOG}
  echo "The ${num} sample ......" >>${LOG}
  date >>${LOG}
  echo "\n" >>${LOG} 

  echo "swap -l " >>${LOG}
  swap -l >>${LOG}
  echo "\n" >>${LOG} 

  echo "vmstat 1 3" >>${LOG}
  vmstat 1 3 >>${LOG}
  echo "\n" >>${LOG} 

  echo "prstat -a 1 3" >>${LOG}
  prstat -a 1 3 >>${LOG}
  echo "\n" >>${LOG} 

  echo "iostat 1 3" >>${LOG}
  iostat 1 3 >>${LOG}
  echo "\n" >>${LOG} 

  num=`expr $num + 1`
  sleep 600
  echo "\n" >>${LOG} 
done

