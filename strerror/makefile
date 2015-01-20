SRCS=\
    main.cpp

OBJS=\
    ${SRCS:.cpp=.o}


TARGS=\
    strerror.exe

.PHONY: all clean


all:${TARGS}

${TARGS}:${OBJS}
	CC -o $@ $< -L/home/bwang1/SunStudioProjects/err -lperr

${OBJS}:${SRCS}
	CC -c $< -o $@

clean:
	-rm *.exe *.o


