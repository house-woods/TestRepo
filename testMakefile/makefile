CC=CC
CPPFLAGS=-g 
SRCS1=$(wildcard src/*.cpp)
OBJS1=$(patsubst %.cpp,%.o,$(SRCS1))
SRCS2=$(wildcard test/*.cpp)
OBJS2=$(patsubst %.cpp,%.o,$(SRCS2))
DEPOBJS2=$(filter-out src/main.o,$(OBJS1))
VPATH=src:test
override CPPFLAGS += $(patsubst %,-I%,$(subst :, ,$(VPATH)))


.PHONY: all src test clean
all: src test

src:proj.exe

proj.exe:$(OBJS1)
	@echo "DEPOBJS2=$(DEPOBJS2)"
	$(CC) -o src/$@ $(OBJS1) 
%.o:%.cpp
	$(CC) -c $< $(CPPFLAGS) -o $@

test:test.exe

test.exe:$(OBJS2)
	$(CC) -o test/$@ $(OBJS2) $(DEPOBJS2) 

clean:
	-rm -rf src/*.o src/proj.exe test/*.o test/test.exe


