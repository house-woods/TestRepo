SRCS=\
   main.cpp
OBJS=${SRCS:.cpp=.o}
TARG=test.exe
.PHONY:all clean
all:${TARG}
${TARG}:${OBJS}
	CC -o $@ $< 

${OBJS}:${SRCS}
	CC -o $@ -c $< -g

clean:
	-rm *.exe *.o

