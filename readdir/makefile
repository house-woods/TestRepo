TARG=rdir.exe
SRCS=main.cpp
OBJS=${SRCS:.cpp=.o}

all:${TARG}

${TARG}:${OBJS}
	CC -o $@ $<
${OBJS}:${SRCS}
	CC -o $@ -c $< -g

clean:
	-rm *.exe *.o

