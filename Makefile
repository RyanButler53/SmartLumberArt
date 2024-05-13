CXX = clang++
CXXFLAGS = -std=c++2a -Wall -Wextra -pedantic -g #-O3
TARGET = smart-lumber

all: $(TARGET)

smart-lumber: smart-lumber.o 
	$(CXX) -o smart-lumber smart-lumber.o

smart-lumber.o: smart-lumber.cpp smart-lumber.hpp
	$(CXX) $(CXXFLAGS) -c smart-lumber.cpp 

clean:
	rm -rf *.o $(TARGET)

clear_art:
	rm -rf *.png
