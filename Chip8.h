#include <stack>
#include <string>
#include <vector>
#include <iostream>
class Chip8 {
private:
  
  static constexpr u_int16_t MEMORY_SIZE = 4096;
  static constexpr int SCREEN_WIDTH = 64;
  static constexpr int SCREEN_HEIGHT = 32;


  static constexpr int CPU_HZ = 700;
  static constexpr double CPU_STEP = 1.0/CPU_HZ;

  static constexpr int TIMER_HZ = 60;
  static constexpr double TIMER_STEP = 1.0/TIMER_HZ;

  static constexpr u_int8_t FONT_START = 0x050;
  static constexpr u_int16_t PROGRAM_START = 0x200;


  static constexpr u_int8_t FONTS[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F

  };

  u_int8_t memory[MEMORY_SIZE];
  u_int16_t pc; //program counter, only needs 12 bits though
  u_int16_t iRegister;
  std::stack<int> stack;

  //registers, each one byte each (8 bits)
  u_int8_t registers[16];
    //timers
  u_int8_t delayTimer;
  u_int8_t soundTimer;

  //display is 64 pixels wide, 32 pixels tall
  int display[SCREEN_HEIGHT][SCREEN_WIDTH];

  //keypad input 
  int keypad[16];


public:

  Chip8(){
    //load instructions into memory

    for (int i = 0; i < 80; i ++){
      memory[FONT_START+i] = FONTS[i];
    }

    pc = PROGRAM_START;
    delayTimer = 100;
    soundTimer = 100;
  }

  void loop(); //main loop
  u_int16_t fetch();
  void decode();
  void execute(u_int16_t instruction);

  //Instructions

  void clearScreen();
  void jump(u_int16_t address);
  void setRegister(u_int8_t reg, u_int8_t value);
  void add(u_int8_t reg, u_int8_t value);
  void setIndexRegister(u_int16_t address);
  void dxyn(u_int8_t xReg, u_int8_t yReg, u_int8_t height);
  void cycle();
  bool loadROM(const std::string& path);
  void drawScreen();

  

};
