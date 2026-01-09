#include "keypad.hpp"
#include <iostream>

void Keypad::setKey(uint8_t key, bool pressed){
    keys[key] = pressed;
}

bool Keypad::isPressed(uint8_t key) const{
    return keys[key];
}

bool Keypad::anyKeyPressed() const{
    for (int i = 0; i < 16; i ++){
        if (keys[i]){
            return true;
        }
    }

    return false;
}

uint8_t Keypad::getKeyPressed() const{
    for (int i = 0; i < 16; i ++){
        if (keys[i]){
            return i;
        }
    }
    return 0xFF;
}

Keypad::Keypad(){
    for (int i = 0; i < 16; i ++){
        keys[i] = 0;
    }
}

