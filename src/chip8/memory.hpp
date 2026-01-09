#pragma once
#include "constants.hpp"
#include <stack>

class Memory {
private:
  uint8_t memory[chip8::MEMORY_SIZE];
  uint16_t iRegister;
  std::stack<uint16_t> stack;
  u_int8_t registers[16];

public:
  Memory();
  void writeMemory(uint16_t address, uint8_t value);
  uint8_t readMemory(uint16_t address) const;
  void setRegister(uint8_t reg, uint8_t value);
  uint8_t getRegister(uint8_t reg) const;
  void setIndexRegister(uint16_t value);
  uint16_t getIndexRegister();
  void loadROM(const std::string &path);
  void pushToStack(uint16_t address);
  uint16_t top();
};
