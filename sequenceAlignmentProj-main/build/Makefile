CXX=g++ -std=c++11
CFLAGS=-c -g
OBJ=-o
MAKE=make

all:
	$(MAKE) efficient
	$(MAKE) basic

efficient: efficient.o
	$(CXX) $(OBJ) prog_efficient efficient.o

basic: basic.o
	$(CXX) $(OBJ) prog_basic basic.o

basic.o: ../src/basic.cpp
	$(CXX) $(CFLAGS) ../src/basic.cpp

efficient.o: ../src/efficient.cpp
	$(CXX) $(CFLAGS) ../src/efficient.cpp

clean:
	rm -f prog_efficient prog_basic *.o
