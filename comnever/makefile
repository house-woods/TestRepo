OBJECTSPACE_HOME = /home/objstadm/ospace2.2.2/ccs-2.2.2

OSPACE_INCLUDES = \
        -I$(OBJECTSPACE_HOME) \
        -I$(OBJECTSPACE_HOME)/ospace/std


OSPACE_DEFINES = \
        -DOS_SOLARIS_2_7 \
        -DOS_STL_ASSERT \
        -DOS_NO_WSTRING \
        -DOS_MULTI_THREADED \
        -DOS_SOLARIS_PTHREADS \
        -DOS_PTHREADS \
        -DOS_REALTIME \
        -D_REENTRANT \
        -D_POSIX_PTHREAD_SEMANTICS \
  -DOS_USE_ALTERNATE_STD \
  -DOS_USE_STD_IOSTREAMS \
        -DTELLABS_EMS_BYPASS_AUTOPTR \
       -DTELLABS_TAO_UNION_SEMUN \

OSPACE_LDFLAGS = \
        -L$(OBJECTSPACE_HOME)/lib \
        -los_std \
        -los_helper \
        -los_time \
        -los_thread \
        -los_io \
        -los_security \
        -los_pipe \
        -los_file \
        -los_stream \
        -los_network \
        -los_unix \
        -los_uss_file \
        -los_uss_network \
        -los_uss_security \
        -los_uss_std \
        -los_uss_helper \
        -los_uss_time \
        -los_uss_unix \
        -lnsl \
        -lsocket \
        -lposix4 \
  -lospace

TARG=token.exe

SRCS=main.cpp

OBJS=${SRCS:.cpp=.o}


.PHONY:all clean

all:${TARG}

${TARG}:${OBJS}
	CC -o $@ $< ${OSPACE_LDFLAGS}

${OBJS}:${SRCS}
	CC -c $< -o $@ -g ${OSPACE_INCLUDES} ${OSPACE_DEFINES}

clean:
	-rm *.o *.exe


