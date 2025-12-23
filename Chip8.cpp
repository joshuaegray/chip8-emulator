#include "Chip8.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <cstdint>

void Chip8::loop(){
    auto lastCpu = std::chrono::steady_clock::now();
    auto lastClock = std::chrono::steady_clock::now();
    while (true){
        auto now = std::chrono::steady_clock::now();
        auto cpuDt = std::chrono::duration<double>(now-lastCpu).count();
        auto clockDt = std::chrono::duration<double>(now-lastClock).count();

        while (cpuDt >= CPU_STEP){
            cycle();
            cpuDt -= CPU_STEP;
            lastCpu += std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(CPU_STEP));
        }

        while (clockDt >= TIMER_STEP){
            if (delayTimer > 0){
                delayTimer --;
            }

            if (soundTimer > 0){
                soundTimer --;
            }

            clockDt -= TIMER_STEP;
            lastClock += std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(TIMER_STEP));
        }        

    }
}



u_int16_t Chip8::fetch(){
    if (pc <= 4094){
        u_int16_t firstByte = memory[pc] << 8;
        pc ++;
        std::cout << "First byte" << int(firstByte) << std::endl;
        u_int16_t secondByte = memory[pc];
        std::cout << "Second byte" << int(secondByte) << std::endl;
        pc ++;
        std::cout << (firstByte | secondByte) << std::endl;
        return (firstByte | secondByte);
    }


    std::cerr << "PC out of bounds" << std::endl;
    exit(1);

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
            if (n == 0){
                clearScreen();
                std::cout << "clear screen" << std::endl;
                break; 
            }

            else if (n == 0xE){
                //return from subroutine
            }

        case 1:
            jump(nnn);
            std::cout << "jump" << std::endl;
            break;
        case 6:
            setRegister(x, nn);
            std::cout << "set register" << std::endl;
            break;
        case 7:
            add (x, nn);
            break;
        case 0xA:
            setIndexRegister(nnn);
            std::cout << "Set index register" << std::endl;
            break;
        case 0xD:
            dxyn(x, y, n);
            std::cout << "dxyn" << std::endl;
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

void Chip8::dxyn(u_int8_t xReg, u_int8_t yReg, u_int8_t height){
    u_int8_t xCoord = registers[xReg] & (SCREEN_WIDTH-1);
    u_int8_t yCoord = registers[yReg] & (SCREEN_HEIGHT-1);
    registers[0xF] = 0;

    for (u_int8_t row = 0; row < height; row ++){
        u_int16_t spriteAddress = iRegister + row;
        u_int8_t y = yCoord + row;
        if (spriteAddress > 0xFFF){
            break;
        }

        if (y >= SCREEN_HEIGHT){
            break;
        }

        u_int8_t spriteByte = memory[spriteAddress];

        for (u_int8_t bit = 0; bit < 8; bit ++){
            u_int8_t x = xCoord + bit;
            if (x >= SCREEN_WIDTH){
                break;
            }

            u_int8_t spritePixel = (spriteByte & (0x80 >> bit)) != 0;
            if (display[y][x] == 1 && (spritePixel)){
                registers[0xF] = 1;
            }
            display[y][x] = display[y][x] ^ spritePixel;
        }
    }



    drawScreen();
}

void Chip8::cycle(){
    u_int16_t instruction = fetch();
    execute(instruction);
}


//Chat GPT 
bool Chip8::loadROM(const std::string& path) {
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
    if (!file.read(reinterpret_cast<char*>(rom.data()), size)) {
        std::cerr << "Failed to read ROM bytes.\n";
        return false;
    }

    if (PROGRAM_START + rom.size() > MEMORY_SIZE) {
        std::cerr << "ROM too large to fit in memory.\n";
        return false;
    }

    for (size_t i = 0; i < rom.size(); i++) {
        memory[PROGRAM_START + i] = rom[i];
    }

    std::cout << "Program loaded succesfully" << std::endl;

    return true;
}


void Chip8::drawScreen(){
    for (int i = 0; i < SCREEN_HEIGHT; i ++){
        for (int j = 0; j < SCREEN_WIDTH; j ++){
            if (display[i][j] == 0){
                std::cout << ".";
            }
            else{
                std::cout << "#";
            }
        }
        std::cout << std::endl;
    }
}
