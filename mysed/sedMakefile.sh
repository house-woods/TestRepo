#!/usr/bin/bash
FILES0=`find . -name "*.mk"|xargs grep "/vobs"|nawk -F: '{print $1}'`
oldfile0=""
for file0 in ${FILES0}
do
    echo "--------------"
    if [ "X${file0}" = "X${oldfile0}" ]
    then
        continue
    else
        echo "newfile: $file0"
        oldfile0=${file0}
        sed 's/\/vobs\/EMS_OTS\//\/home\/bwang1\/src\/EMS_OTS\//g' $file0 >${file0}.new
        mv ${file0} ${file0}.old
        mv ${file0}.new ${file0}
    fi
done


FILES=`find . -name "Makefile"|xargs grep "/vobs"|nawk -F: '{print $1}'`
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
        sed 's/\/vobs\/EMS_OTS\//\/home\/bwang1\/src\/EMS_OTS\//g' $file >${file
}.new
        mv ${file} ${file}.old
        mv ${file}.new ${file}
    fi
done

