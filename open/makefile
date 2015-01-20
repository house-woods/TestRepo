SRCS=\
    main.cpp


OBJS=${SRCS:.cpp=.o}


TARG=open.exe


.PHONY:all clean


all:${TARG}

${TARG}:${OBJS}
	CC -o $@ $< -L/home/bwang1/SunStudioProjects/err -lperr


${OBJS}:${SRCS} 
	CC -o $@ -c $<


clean:
	-rm *.exe *.o


