#pragma once
#include "../platform/display.hpp"
#include "constants.hpp"
#include "memory.hpp"
#include "timers.hpp"

class CPU {

public:
  void cycle(Memory &memory, Display &display);
  CPU();

private:
  uint16_t pc;
  void loop(Timer &timers, Memory &memory, Display &display);
  uint16_t fetch(Memory &memory);
  void decode();
  void execute(uint16_t instruction, Display &display, Memory &memory);

  // opcode instructions
  void clearScreen(Display &display);
  void jump(u_int16_t address);
  void setRegister(u_int8_t reg, u_int8_t value, Memory &memory);
  void add(u_int8_t reg, u_int8_t value, Memory &memory);
  void setIndexRegister(u_int16_t value, Memory &memory);
  void dxyn(u_int8_t xReg, u_int8_t yReg, u_int8_t height, Memory &memory, Display &display);
  void startSubroutine(uint16_t address, Memory& memory);
  void returnFromSubroutine(Memory& memory);
};
