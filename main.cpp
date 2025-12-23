#include "Chip8.h"
#include <iostream>

int main(int argc, char* argv[]){
    if (argc != 2){
        std::cerr << "Usage: ./main <file path>" << std::endl;
        exit(1);
    }

    Chip8 myEmulator;
    myEmulator.loadROM(argv[1]);

    for (int i = 0; i < 11; i ++){
        myEmulator.cycle();
    }

    return 0;
}