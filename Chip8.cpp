#include "Chip8.h"
#include <chrono>
#include <iostream>

void Chip8::loop(){
    auto last = std::chrono::steady_clock::now();
    double timerAccumulator = 0.0;
    double cpuAccumulator = 0.0;
    while (true){
        auto now = std::chrono::steady_clock::now();
        auto timeDifference = std::chrono::duration<double>(now-last).count();
        last = now;
        timerAccumulator+= timeDifference;

        cpuAccumulator += timeDifference * CPU_HZ;

        while (cpuAccumulator >= 1.0){
            cycle();
            cpuAccumulator --;
        }

        while (timerAccumulator >= TIMER_STEP){
            timerAccumulator -= TIMER_STEP;
            if (delayTimer > 0){
                delayTimer --;
            }

            if (soundTimer > 0){
                soundTimer --;
                //std::cout << "\a";
                //todo: implement sound
            }
        }
        

    }


}



u_int16_t Chip8::fetch(){
    u_int8_t firstByte = memory[pc] << 8;
    pc ++;
    u_int8_t secondByte = memory[pc];
    pc ++;
    return (firstByte | secondByte);
}

void Chip8::execute(u_int16_t instruction){
    u_int8_t opcode = (instruction & 0xF000) >> 12; //first nibble
    u_int8_t x = (instruction & 0x0F00) >> 8; //second nibble
    u_int8_t y = (instruction & 0x00F0) >> 4; //third nibble
    u_int8_t n = (instruction & 0x000F);  //fourth nibble
    u_int8_t nn = (instruction & 0x00FF); //second byte (third+fourth nibble)
    u_int16_t nnn = (instruction & 0x0FFF); //second, third, fourth, nibbles 


    switch (opcode){
        case 0:
            clearScreen();
            break; 
        case 1:
            jump(nnn);
            break;
        case 6:
            setRegister(x, nn);
            break;
        case 7:
            add (x, nn);
            break;
        case 0xA:
            setIndexRegister(nnn);
            break;
        case 13:
            drawDisplay(x, y, n);
            break;
    }
}

void Chip8::clearScreen(){
    for (int i = 0; i < 32; i ++){
        for (int j = 0; j < 64; j ++){
            display[i][j] = 0;
        }
    }
}

void Chip8::jump(u_int16_t address){
    pc = address;
}

void Chip8::setRegister(u_int8_t reg, u_int8_t value){
    registers[reg] = value;
}

void Chip8::add(u_int8_t reg, u_int8_t value){
    registers[reg] += value;
}


void Chip8::setIndexRegister(u_int16_t address){
    iRegister = address;
}

void Chip8::drawDisplay(u_int8_t xReg, u_int8_t yReg, u_int8_t height){
    u_int8_t xCoord = registers[xReg] & (SCREEN_WIDTH-1);
    u_int8_t yCoord = registers[yReg] & (SCREEN_HEIGHT-1);
    registers[0xF] = 1;

    int counter = 0;
    int currentRow;
    while (counter < height && yCoord < SCREEN_HEIGHT){
        currentRow = memory[iRegister + counter];
        for (int i = 0; i < 8; i ++){
            if (xCoord + i < SCREEN_WIDTH){
                display[yCoord][xCoord + i] ^ (currentRow & 0b10000000);
                if (display[yCoord][xCoord] == 0 && (currentRow & 0b10000000 == 1)){
                    registers[0xF] = 1;
                }
                currentRow = currentRow << 1; 
            }
            
            else{
                break;
            }
        }
    }

}





