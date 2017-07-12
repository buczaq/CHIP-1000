CFLAGS=-Wall -lboost_unit_test_framework -std=c++14 -lstdc++

chip: chip8.cpp chip8.hpp tests.cpp
	g++ -o chip1000 chip8.cpp chip8.hpp main.cpp $(CFLAGS)
