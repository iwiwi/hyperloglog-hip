CXXFLAGS = -g -std=c++0x -O3 -Wall -Wextra -pthread -I src

.PHONY: test clean

bin/sample: src/*.h sample/sample.cc
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/test: src/* gtest/*.cc
	$(CXX) $(CXXFLAGS) -o $@ $^

bin:
	mkdir -p bin

test: bin bin/test
	bin/test

clean:
	rm -rf bin
