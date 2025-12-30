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
  void clearScreen(Display &display); //00E0
  void jump(u_int16_t address); //1NNN
  void setRegister(u_int8_t reg, u_int8_t value, Memory &memory); //6XNN
  void add(u_int8_t reg, u_int8_t value, Memory &memory); //7XNN
  void setIndexRegister(u_int16_t value, Memory &memory); //ANNN
  void dxyn(u_int8_t xReg, u_int8_t yReg, u_int8_t height, Memory &memory, Display &display); //DXYN
  void startSubroutine(uint16_t address, Memory& memory); //2NNN
  void returnFromSubroutine(Memory& memory);  //00EE

  void skipIfEqual(uint8_t reg, uint16_t value, Memory& memory); //3XNN
  void skipIfNotEqual(uint8_t reg, uint16_t value, Memory& memory); //4XNN
  void skipIfRegEqual(uint8_t xReg, uint8_t yReg, Memory& memory); //5XY0
  void skipIfRegNotEqual(uint8_t xReg, uint8_t yReg, Memory& memory); //9XY0


  void setRegToReg(uint8_t xReg, uint8_t yReg, Memory& memory); //8XY0

  void binaryOR(uint8_t xReg, uint8_t yReg, Memory& memory); //8XY1

  void binaryAND(uint8_t xReg, uint8_t yReg, Memory& memory); //8XY2

  void logicalXOR(uint8_t xReg, uint8_t yReg, Memory& memory); //8XY3

  void addRegs(uint8_t xReg, uint8_t yReg, Memory& memory); //8XY4

  void subtractYFromX(uint8_t xReg, uint8_t yReg, Memory& memory); //8XY5

  void subtractXFromY(uint8_t xReg, uint8_t yReg, Memory& memory); //8XY7

  //Note: My implementation ignores the yReg
  void shiftRight(uint8_t xReg, uint8_t yReg, Memory& memory); //8XY6
  void shiftLeft(uint8_t xReg, uint8_t yReg, Memory& memory); //8XYE





};
