@echo off
set TRAP_DIR=.
set JAVA_HOME=C:\Program Files\Java\jdk1.5.0_12
set PATH="%TRAP_DIR%";"%TRAP_DIR%\adventnet";%PATH%

set CLASSPATH=.;"%TRAP_DIR%\adventnet";"%TRAP_DIR%\adventnet\AdventNetLogging.jar";"%TRAP_DIR%\adventnet\AdventNetSnmp.jar"

::start "Tellabs EMS 7100" /B "%JAVA_HOME%\bin\javaw" -XX:MaxNewSize=8m -XX:+UseParallelGC -Xmx768m -cp %CLASSPATH% snmpv3trap -p 162 -e 0x12345678 -a HMAC-MD5 -w 7100EMS01 -pp DES -s 7100EMS01 -n bobo EMS7100 192.168.121.202 1111 1.3.6.1.6.3.1.1.5.3 1.3.6.1.2.1.2.2.1.1 INTEGER 88 1.3.6.1.2.1.2.2.1.7 INTEGER 1 1.3.6.1.2.1.2.2.1.8 INTEGER 2

java -cp %CLASSPATH% snmpv3trap -p 162 -e 0x8000057504584930303130303031 -a HMAC-MD5 -w 7100EMS01 -pp DES -s 7100EMS01 -n bobo EMS7100 127.0.0.1 1111 1.3.6.1.6.3.1.1.5.3 1.3.6.1.2.1.2.2.1.1 INTEGER 88 1.3.6.1.2.1.2.2.1.7 INTEGER 1 1.3.6.1.2.1.2.2.1.8 INTEGER 2
