#include "chip8.hpp"
#include "display.hpp"
#include "keypad.hpp"
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

const Display& Chip8::getDisplay() const{
    return display;
}

Keypad& Chip8::getKeypad(){
    return keypad;
}

bool Chip8::updateTimers(){
    if (timer.getDelayTimer() > 0){
        timer.decrementDelayTimer();
    }

    if (timer.getSoundTimer() > 0){
        timer.decrementSoundTimer();
        return true;
    }

    return false;
}
