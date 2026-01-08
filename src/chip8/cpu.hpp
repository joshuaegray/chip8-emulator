#pragma once
#include "display.hpp"
#include "constants.hpp"
#include "memory.hpp"
#include "timers.hpp"
#include "keypad.hpp"

class CPU {

public:
  void cycle(Memory &memory, Display &display, Keypad& keypad, Timer& timer);
  CPU();

private:
  uint16_t pc;
  void loop(Timer& timer, Memory& memory, Display& display, Keypad& keypad);
  uint16_t fetch(Memory& memory);
  void decode();
  void execute(uint16_t instruction, Display& display, Memory& memory, Keypad& keypad, Timer& timer);

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

  //untested
  void jumpWithOffset(uint16_t address, Memory& memory); //BNNN (pc = NNN + V0)
  void random(uint8_t reg, uint8_t nn, Memory& memory); //CXNN

  void skipIfPressed(uint8_t reg, Keypad& keypad, Memory& memory); //EX9E
  void skipIfNotPressed(uint8_t reg, Keypad& keypad, Memory& memory); //EXA1

  void getDelayTimer(uint8_t reg, Timer& timer, Memory& memory) const; //FX07
  void setDelayTimer(uint8_t reg, Timer& timer, Memory& memory) const; //FX15
  void setSoundTimer(uint8_t reg, Timer& timer, Memory& memory) const; //FX18

  void addToIndex(uint8_t reg, Memory& memory) const; //FX1E

  void getKey(uint8_t reg, Keypad& keypad, Memory& memory); //FX0A

  void fontCharacter(uint8_t reg, Memory& memory); //FX29

  void binaryToDecimal(uint8_t reg, Memory& memory); //FX33

  void loadMemory(uint8_t reg, Memory& memory); //FX55

  void storeMemory(uint8_t reg, Memory& memory); //FX65







};
