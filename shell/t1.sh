#!/bin/ksh
SOURCEDIR=$1
NBIBackupDir=$2
if [[ ${SOURCEDIR} = ${NBIBackupDir} || `expr "${NBIBackupDir}" : "${SOURCEDIR}/"` -gt 0 ]]
then
  /bin/echo "\n backup dir cannot be the same as [${SOURCEDIR}] \n and cannot be contained by [${SOURCEDIR}]."
fi
echo ${NBIBackupDir}
MIGRATEDIR=`echo ${NBIBackupDir} | sed -e 's/Tellabs_7190_EMS\/..\/**//g'`
echo "MIGRATEDIR=${MIGRATEDIR}"

MIGRATEDIR=`echo ${NBIBackupDir} | sed -e 's/Tellabs_7190_EMS\/..//g'`
echo "MIGRATEDIR=${MIGRATEDIR}"
