TARG=libperr.so
SRCS= \
    err.c\
    log.c
OBJ=$(SRCS:.c=.o)

.PHONY : all clean

all: ${TARG}

${TARG}: ${OBJ} 
	cc -o $@ $< -shared 

%.o: %.cpp
	cc -g -c $< -o $@


clean:
	-rm *.o *.so


