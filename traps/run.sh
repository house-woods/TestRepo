#!/bin/ksh
TRAP_DIR=.
JAVA_HOME=./jre
PATH=$PATH:$TRAP_DIR:$TRAP_DIR/adventnet

CLASSPATH=.:$TRAP_DIR/adventnet:$TRAP_DIR/adventnet/AdventNetLogging.jar:$TRAP_DIR/adventnet/AdventNetSnmp.jar

export PATH 
export CLASSPATH


${JAVA_HOME}/bin/java -cp $CLASSPATH snmpv3trap -p 162 -e 0x8000057504584930303130303031 -a HMAC-MD5 -w 7100EMS01 -pp DES -s 7100EMS01 -n bobo EMS7100 192.168.121.202 1111 1.3.6.1.6.3.1.1.5.3 1.3.6.1.2.1.2.2.1.1 INTEGER 88 1.3.6.1.2.1.2.2.1.7 INTEGER 1 1.3.6.1.2.1.2.2.1.8 INTEGER 2

