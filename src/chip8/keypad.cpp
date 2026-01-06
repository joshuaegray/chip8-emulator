#include "keypad.hpp"

void Keypad::setKey(uint8_t key, bool pressed){
    keys[key] = true;
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

