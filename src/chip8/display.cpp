#include "display.hpp"
#include <iostream>
void Display::drawScreen(){
    std::cout << "----------------------------" << std::endl;
    for (int i = 0; i < chip8::SCREEN_HEIGHT; i ++){
        for (int j = 0; j < chip8::SCREEN_WIDTH; j ++){
            if (display[i][j] == 0){
                std::cout << ".";
            }
            else{
                std::cout << "#";
            }
        }
        std::cout << std::endl;
    }
}

Display::Display(){
    for (int i = 0; i < chip8::SCREEN_HEIGHT; i ++){
        for (int j = 0; j < chip8::SCREEN_HEIGHT; j ++){
            display[i][j] = 0;
        }
    }
}

void Display::turnPixelOff(uint8_t y, uint8_t x){
    display[y][x] = 0;
}

int Display::getPixel(uint8_t y, uint8_t x) const{
    return display[y][x];
}

void Display::turnPixelOn(uint8_t y, uint8_t x){
    display[y][x] = 1;
}
