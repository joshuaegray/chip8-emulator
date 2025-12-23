CXX = g++ -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

emulate: Chip8.o main.o Chip8.h 
	${CXX} Chip8.o main.o -o emulate

Chip8.o: Chip8.cpp Chip8.h
	${CXX} Chip8.cpp -c

main.o: main.cpp
	${CXX} main.cpp -c

clean:
	rm -f *.o emulate
