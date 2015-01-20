#!/usr/bin/bash

FILES=`find . -name "*.mk"|xargs grep "/vobs"|nawk -F: '{print $1}'`
oldfile=""
for file in ${FILES}
do
    echo "--------------"
    if [ "X${file}" = "X${oldfile}" ]
    then
        continue 
    else
        echo "newfile: $file"
        oldfile=${file}
        sed 's/\/vobs\/EMS_OTS\//\/home\/bwang1\/src\/EMS_OTS\//g' $file >${file}.new
        mv ${file} ${file}.old
        mv ${file}.new ${file} 
    fi
done






