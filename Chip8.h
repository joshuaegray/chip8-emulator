#include <stack>
class Chip8 {
private:
  
  static constexpr u_int16_t MEMORY_SIZE = 4096;
  static constexpr int SCREEN_WIDTH = 64;
  static constexpr int SCREEN_HEIGHT = 32;


  static constexpr int CPU_HZ = 700;

  static constexpr int TIMER_HZ = 60;
  static constexpr double TIMER_STEP = 1.0/TIMER_HZ;

  static constexpr u_int8_t FONT_START = 0x050;

  //static constexpr u_int8_t FONTS[80] = {
    //todo
 // }

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
  void drawDisplay(u_int8_t xReg, u_int8_t yReg, u_int8_t height);
  void cycle();

  

};
