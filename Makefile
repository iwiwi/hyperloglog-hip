CXX = g++
CXXFLAGS = -g -std=c++0x -O3 -Wall -Wextra -pthread -I.

.PHONY: test clean

test: bin bin/test
	bin/test

clean:
	rm -rf bin

bin:
	mkdir -p bin

bin/test: src/* gtest/*.cc
	$(CXX) $(CXXFLAGS) -o $@ $^
