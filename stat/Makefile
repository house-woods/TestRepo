SRCS=\
    main.c


OBJS=${SRCS:.c=.o}


TARG=stat.exe


.PHONY:all clean

all:${TARG}


${TARG}:${OBJS}
	cc -o $@ $< -L/home/bwang1/SunStudioProjects/err -lperr

${OBJS}:${SRCS}
	cc -o $@ -c $< 

clean:
	-rm *.exe *.o
