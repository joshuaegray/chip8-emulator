#include "chip8.hpp"
#include <string>
void Chip8::cycle() { 
    cpu.cycle(memory, display, keypad, timer); 
}

void Chip8::loadROM(const std::string &path){
    memory.loadROM(path);
}



Chip8::Chip8(){
    //pass
}

