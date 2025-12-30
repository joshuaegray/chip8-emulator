#include "chip8.hpp"
#include <iostream>

int main(int argc, char* argv[]){
    if (argc != 2){
        std::cerr << "Usage: ./main <file path>" << std::endl;
        exit(1);
    }

    Chip8 myEmulator;
    myEmulator.loadROM(argv[1]);

    for (int i = 0; i < 15; i ++){
        myEmulator.cycle();
    }

    return 0;
}