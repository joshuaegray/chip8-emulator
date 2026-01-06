#pragma once
#include "display.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include "keypad.hpp"
#include "timers.hpp"
#include <string>
class Chip8 {
private:
  Memory memory;
  CPU cpu;
  Display display;
  Keypad keypad;
  Timer timer;

public:
  Chip8();
  void loadROM(const std::string &path);
  void cycle();

  // Instructions
};

