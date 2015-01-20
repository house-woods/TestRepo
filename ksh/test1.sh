#!/usr/bin/bash
 echo -e "Input your operate system:\c"
 read answer;

 function func
 {
 echo "Function start>>>>>>"
     if [ "$answer" != "`uname`" ]
     then
         echo $answer
         return "1"
     else
         echo $answer
         return "0"
     fi
 }
 #ret=`func`
 func
 echo "return value:$?" 
 #echo "*** $ret ***"
