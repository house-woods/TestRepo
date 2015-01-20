SRCS=\
    main.cpp

OBJS=${SRCS:.cpp=.o}


TARG=pathconf.exe


.PHONY:all clean




all:${TARG}

${TARG}:${OBJS}
	CC -o $@ $< -L/home/bwang1/SunStudioProjects/err -lperr


${OBJS}:${SRCS}
	CC -c $< -o $@



clean:
	-rm *.o ${TARG}

