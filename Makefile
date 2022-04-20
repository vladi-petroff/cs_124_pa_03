.PHONY: all clean

CXX=g++
CXXFLAGS=-std=c++17 -Wall -pedantic -Wextra -lm -lpthread


all: partition

partition: partition.cpp
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude $<

clean:
	rm -rf partition