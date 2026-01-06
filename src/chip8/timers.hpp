#pragma once
#include "constants.hpp"
#include <chrono>

class Timer {
private:
  uint8_t delayTimer;
  uint8_t soundTimer;

public:
  Timer();
  uint8_t getDelayTimer();
  uint8_t getSoundTimer();
  void decrementDelayTimer();
  void decrementSoundTimer();
  void setDelayTimer(uint8_t value);
  void setSoundTimer(uint8_t value);
};
