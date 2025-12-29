#pragma once
#include "../chip8/constants.hpp"

class Display {
private:
  int display[chip8::SCREEN_HEIGHT][chip8::SCREEN_WIDTH];
  //uint8_t keypad[16]; implement later

public:
  Display();
  void turnPixelOff(uint8_t y, uint8_t x);
  int getPixel(uint8_t y, uint8_t x);
  void turnPixelOn(uint8_t y, uint8_t x);
  void drawScreen();
};
