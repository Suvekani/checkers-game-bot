CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2 -Iinclude

HEADERS  := $(wildcard include/*.h)

all: checkers

checkers: src/main.cpp src/board.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) src/main.cpp src/board.cpp -o checkers

test_board: tests/test_board.cpp src/board.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) tests/test_board.cpp src/board.cpp -o test_board

test: test_board
	./test_board

clean:
	$(RM) checkers checkers.exe test_board test_board.exe

.PHONY: all test clean
