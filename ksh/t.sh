#!/bin/ksh

> prstatReportJ
> prstatReportU
#set -x
while [ `du -sk prstatReportJ | awk '{print $1}'` -lt  2 ]
do
            date>>prstatReportJ
            prstat -n 5 -J -U bwang1 6000 2 >> prstatReportJ
            date>>prstatReportU
            prstat -t -U bwang1 6000 2 >> prstatReportU
done

