#include "memory.hpp"
#include "constants.hpp"
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
  if (address >= 0 && address < chip8::MEMORY_SIZE){
    memory[address] = value;
  }

  else{
    std::cerr << "Write Memory Error: Memory out of bounds" << std::endl;
    exit(1);
  }
}

uint8_t Memory::readMemory(uint16_t address) const{
  if (address >= 0 && address < chip8::MEMORY_SIZE){
    return memory[address];
  }

  else{
    std::cerr << "Read Memory Error: Memory out of bounds" << std::endl;
    exit(1);
  }
}

void Memory::setRegister(uint8_t reg, uint8_t value){
  if (reg >= 0 && reg <= 15){
    registers[reg] = value;
  }

  else{
    std::cerr << "Set Register Memory: Register out of bounds" << std::endl;
    exit(1);
  }
}

uint8_t Memory::getRegister(uint8_t reg) const{
  if (reg >= 0 && reg <= 15){
    return registers[reg];
  }
  else{
    std::cerr << "Get Register memory: Register out of bounds" << std::endl;
    exit(1);
  }
}

void Memory::setIndexRegister(uint16_t value){
  if (value >= 0 && value < chip8::MEMORY_SIZE){
    iRegister = value;
  }

  else{
    std::cerr << "Set Index Register Error: Address out of bounds" << std::endl;
    exit(1);
  }
}

uint16_t Memory::getIndexRegister(){
  return iRegister;
}

void Memory::pushToStack(uint16_t address){
  if (stack.size() == 16){
    std::cerr << "Error: Stack size is too large" << std::endl;
    exit(1);
  }
  stack.push(address);
}

uint16_t Memory::top(){
  if (stack.empty()){
    std::cerr << "Error: Cannot pop from empty stack" << std::endl;
    exit(1);
  }
  uint16_t returnValue = stack.top();
  stack.pop();
  return returnValue;
}
