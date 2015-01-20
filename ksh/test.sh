#!/bin/ksh


function test1
{
   cnt=`expr 1 + 1`
   echo $cnt
   return 1
}

res=test1

ret=$?

echo "res=$res"
echo "ret=$ret"

cd ~
curdir=`pwd`
echo ${curdir}

