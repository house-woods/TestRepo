SRC=main.cpp
TARG=deamon
OBJ=$(SRC:.cpp=.o) 


$(TARG):$(OBJ)
	CC -o $@ $<
$(OBJ):$(SRC)
	CC -o $@ -c $<
clean:
	-rm -rf *.o deamon

