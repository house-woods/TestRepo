#PDIR=/home/addon/rational/releases/PurifyPlusFamily.2003.06.00/sun4_solaris2/bin
#PDIR=/home/addon/rational/releases/PurifyPlusFamily.2002a.06.00/sun4_solaris2/bin
#PDIR=/home/addon/rational/releases/purify.sol.7.0
PDIR=/home/addon/rational/releases/purify.sol.2003.06.00

TARG=testQ.exe

SRCS=main.cpp

OBJS=${SRCS:.cpp=.o}

CC=/home/addon/Sun_Studio_11/SUNWspro/bin/CC

.PHONY: all clean

all:${TARG}

${TARG}:${OBJS}
	${PDIR}/purify -follow_terminal_symlinks -cache-dir=/home/bwang1/SunStudioProjects/cache ${CC} -o $@ ${OBJS} 
${OBJS}:${SRCS}
	${PDIR}/purify -follow_terminal_symlinks -cache-dir=/home/bwang1/SunStudioProjects/cache ${CC} -c ${SRCS} -o $@ -g 

clean:
	-rm *.exe *.o *.warnings

