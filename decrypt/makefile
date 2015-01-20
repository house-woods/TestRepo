include /vobs/EMS_OTS/GlobalRules.mk
SRCS=\
    main.cpp


OBJS=${SRCS:.cpp=.o}


TARG=decrypt


.PHONY:all clean

all:${TARG}

${TARG}:${OBJS}
	CC -o $@ $< -L${FRAMEWORK_ROOT}/libs -lfw -ltl1 -lidl -ldbms -lelemmgr -lsnmp ${ORB_LDFLAGS}\
${OSPACE_LDFLAGS} ${SNMP_PP_LDFLAGS} ${ORACLE_LDFLAGS} ${DES_LDFLAGS} -mt 

${OBJS}:${SRCS}
	CC -c $< -o $@ -g -I/home/objstadm/ospace2.2.2/ccs-2.2.2 -I/home/objstadm/ospace2.2.2/ccs-2.2.2/ospace/std \
-I/vobs/EMS_OTS/idl -I/vobs/EMS_OTS/Framework/libs/libidl -I/vobs/EMS_OTS/Framework/libs/libfw ${ORB_INCLUDES} ${OSPACE_DEFINES} ${SSL_INCLUDES}



clean:
	-rm *.o ${TARG}

