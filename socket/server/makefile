ACE_ROOT := /vobs/cots/TAO/ACE_wrappers
TAO_ROOT := $(ACE_ROOT)

#CXX = /home/addon/Sun_Studio_11/SUNWspro/bin/CC
CXX = /home/addon/Sun_Studio_12_Sep_12_07/SUNWspro/bin//CC

INCLUDES = \
  -I$(ACE_ROOT) \
  -I$(ACE_ROOT)/include \
  -I$(ACE_ROOT)/include/ace \
  -I$(ACE_ROOT)/include/tao 

LDFLAGS = \
  -L$(ACE_ROOT)/lib \
  -lACE 

#all :
#	$(CXX) $(LDFLAGS) $(INCLUDES) server.cpp -o a.out

TARG=u64toa.exe
SRCS=main.cpp
OBJ=${SRCS:.cpp=.o}

.PHONY: all clean

all: ${TARG}

${TARG}:${OBJ}
	${CXX} ${LDFLAGS} $< -o $@
${OBJ}:${SRCS}
	${CXX} ${INCLUDES} -g -mt -c $< -o $@ 

clean:
	-rm *.exe *.o

