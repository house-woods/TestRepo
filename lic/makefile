SRCS=\
    main.cpp


OBJS=$(SRCS:.cpp=.o)


TARG=lic.exe

.PHONY: all clean

all: $(TARG)

$(TARG):$(OBJS)
	CC -o $(TARG) $(OBJS)

$(OBJS):$(SRCS)
	CC -g -c $(SRCS)

clean:
	-rm *.exe *.o



