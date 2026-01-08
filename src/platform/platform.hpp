#pragma once
#include <SDL2/SDL.h>
#include "../chip8/display.hpp"
#include "../chip8/keypad.hpp"
#include "../chip8/chip8.hpp"

class SdlPlatform {
public:
    SdlPlatform(int windowWidth, int windowHeight);
    ~SdlPlatform();

    void run(Chip8& chip8);

private:
    //void initSDL();
    //void pollInput(Keypad& keypad); not implemented yet
    //void updateTiming(Chip8& chip8);
    void render(const Display& display);
    //void cleanup();

private:
    bool quit = false;

    // SDL objects
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_AudioDeviceID audioDevice;

    void processInput(Keypad& keypad);
    void updateSound(bool shouldPlay);

    // Timing
    //uint64_t lastTime = 0;
    //double cpuAccumulator = 0.0;
    //double timerAccumulator = 0.0;
};
