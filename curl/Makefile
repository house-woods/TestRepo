SRCS=\
    ftpget.cpp


OBJS=${SRCS:.cpp=.o}


TARG=myftp.exe


.PHONY:all clean

all:${TARG}


${TARG}:${OBJS}
	CC -o $@ $< -L/home/objstadm/curl/lib -lcurl

${OBJS}:${SRCS}
	CC -o $@ -c $< -g -I/home/objstadm/curl/include

clean:
	-rm *.exe *.o
