.PHONY : all
all : main.o
	CC -o proc.exe main.o
%.o : %.cpp
	CC -c $< -g -o $@

.PHONY : clean
clean :
	rm *.exe *.o
