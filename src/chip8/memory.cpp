#include "memory.hpp"
#include <fstream>
#include <iostream>

// Chat GPT
bool Memory::loadROM(const std::string &path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) {
    std::cerr << "Failed to open ROM: " << path << "\n";
    return false;
  }

  std::streamsize size = file.tellg();
  if (size <= 0) {
    std::cerr << "ROM file is empty.\n";
    return false;
  }

  file.seekg(0, std::ios::beg);

  std::vector<uint8_t> rom(static_cast<size_t>(size));
  if (!file.read(reinterpret_cast<char *>(rom.data()), size)) {
    std::cerr << "Failed to read ROM bytes.\n";
    return false;
  }

  if (chip8::PROGRAM_START + rom.size() > chip8::MEMORY_SIZE) {
    std::cerr << "ROM too large to fit in memory.\n";
    return false;
  }

  for (size_t i = 0; i < rom.size(); i++) {
    memory[chip8::PROGRAM_START + i] = rom[i];
  }

  std::cout << "Program loaded succesfully" << std::endl;

  return true;
}

Memory::Memory() {
  for (int i = 0; i < 80; i++) {
    memory[chip8::FONT_START + i] = chip8::FONTS[i];
  }
}

void Memory::writeMemory(uint16_t address, uint8_t value){
  memory[address] = value;
}

uint8_t Memory::readMemory(uint16_t address) const{
  return memory[address];
}

void Memory::setRegister(uint16_t reg, uint8_t value){
  registers[reg] = value;
}

uint8_t Memory::getRegister(uint16_t reg) const{
  return registers[reg];
}

void Memory::setIndexRegister(uint16_t value){
  iRegister = value;
}

uint16_t Memory::getIndexRegister(){
  return iRegister;
}

void Memory::pushToStack(uint16_t address){
  stack.push(address);
}

uint16_t Memory::top(){
  uint16_t returnValue = stack.top();
  stack.pop();
  return returnValue;
}
