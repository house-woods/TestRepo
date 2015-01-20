#!/bin/bash

# Author: Aaron.Cheng
# Date:  2007-1-10
   # Check

# Core 
#  echo’‘ﬁ*W&2¶
varCnt()
{
  var=$1   ret=0
  if [ "$var" != "" ] ; then
    ret=${#var}  
  fi
  echo $ret
}

main()
{
  printf "varCnt's 's return value is %s "  `varCnt ""`
  printf "varCnt's 's return value is %s "  `varCnt "1234"`
}
#main

eval "echo "hello"| tee -a test.log"

