#include "chip8.hpp"
#include "platform.hpp"
#include <iostream>

int main(int argc, char* argv[]){
    if (argc != 2){
        std::cerr << "Usage: ./main <file path>" << std::endl;
        exit(1);
    }

    Chip8 myEmulator;
    SdlPlatform platform(640, 320);
    myEmulator.loadROM(argv[1]);

    platform.run(myEmulator);

    return 0;
}