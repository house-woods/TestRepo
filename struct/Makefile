SRCS=\
     tstruct.cpp


OBJS=${SRCS:.cpp=.o}


TARG=stru.exe


.PHONY:all clean

all:${TARG}


${TARG}:${OBJS}
	CC -o $@ $< 

${OBJS}:${SRCS} mys.h
	CC -o $@ -c $< 

clean:
	-rm *.exe *.o
