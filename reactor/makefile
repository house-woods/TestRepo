ACE_ROOT := /vobs/cots/TAO/ACE_wrappers
TAO_ROOT := $(ACE_ROOT)

CXX = /home/addon/Sun_Studio_11/SUNWspro/bin/CC

INCLUDES = \
  -I$(ACE_ROOT) \
  -I$(ACE_ROOT)/include \
  -I$(ACE_ROOT)/include/ace \
  -I$(ACE_ROOT)/include/tao

LDFLAGS = \
  -L$(ACE_ROOT)/lib \
  -lACE \
  -lACE_SSL \
  -lsocket

#all :
#       $(CXX) $(LDFLAGS) $(INCLUDES) test.cpp -o a.out

TARG=reactor.exe
SRCS=main.cpp
OBJ=${SRCS:.cpp=.o}

.PHONY: all clean

all: ${TARG}

${TARG}:${OBJ}
	${CXX} ${LDFLAGS} $< -o $@
${OBJ}:${SRCS}
	${CXX} ${INCLUDES} -g -mt -c $< -o $@

clean:
	-rm -rf *.exe *.o core
