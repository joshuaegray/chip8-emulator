#pragma once
#include "../platform/display.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include <string>
class Chip8 {
private:
  Memory memory;
  CPU cpu;
  Display display;

public:
  Chip8();
  void loadROM(const std::string &path);
  void cycle();

  // Instructions
};

