#include "timers.hpp"

Timer::Timer(){
    delayTimer = 100;
    soundTimer = 100;
}

uint8_t Timer::getDelayTimer(){
    return delayTimer;
}

uint8_t Timer::getSoundTimer(){
    return soundTimer;
}

void Timer::decrementDelayTimer(){
    delayTimer -= 1;
}

void Timer::decrementSoundTimer(){
    soundTimer -= 1;
}

void Timer::setDelayTimer(uint8_t value){
    delayTimer = value;

}

void Timer::setSoundTimer(uint8_t value){
    soundTimer = value;
}