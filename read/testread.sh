#!/usr/bin/bash
cat file | while read line
do
echo $line
echo " :: Please input any key(s):c"
str4read=""
while true
do
chr4read=`dd if=/dev/tty bs=1 count=1 2>/dev/null`
str4read=$str4read$chr4read
if [ "$chr4read" = "" ] ;then break; fi
done
echo " :: |$str4read|"
done
