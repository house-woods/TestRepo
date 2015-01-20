CC:=CC
CPPFLAGS:=-g 
SRCDIR:=src test
SRCS:=$(foreach d,$(SRCDIR),$(wildcard $(d)/*.cpp))
OBJS:=$(patsubst %.cpp,%.o,$(SRCS))
VPATH=src:test
override CPPFLAGS += $(patsubst %,-I%,$(subst :, ,$(VPATH)))


.PHONY: all src clean
all: src 

src:proj.exe

proj.exe:$(OBJS)
	@echo $(CC)
	$(CC) -o src/$@ $(OBJS) 
%.o:%.cpp
	$(CC) -c $< $(CPPFLAGS) -o $@

clean:
	-rm -rf src/*.o src/proj.exe test/*.o


