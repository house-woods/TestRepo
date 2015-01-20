#!/bin/ksh
ConvertKBytesOutput="`echo \"scale=2; 2000000000/ 1000000000\" | bc -l`"
echo ${ConvertKBytesOutput}
