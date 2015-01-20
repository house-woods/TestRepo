TARG=st.exe
SRCS=main.c
OBJS=${SRCS:.c=.o}

all:$(TARG)

$(TARG):$(OBJS)
	cc -o $@ $<

$(OBJS):$(SRCS)
	cc -o $@ -c $< -g


clean:
	-rm *.exe *.o

