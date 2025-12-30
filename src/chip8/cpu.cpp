#include <iostream>
#include "memory.hpp"
#include "cpu.hpp"
#include "constants.hpp"

CPU::CPU(){
    pc = chip8::PROGRAM_START;
}

uint16_t CPU::fetch(Memory& memory){
    if (pc < chip8::MEMORY_SIZE - 1){
        u_int16_t firstByte = memory.readMemory(pc) << 8;
        u_int16_t secondByte = memory.readMemory(pc+1);
        pc += 2;
        return (firstByte | secondByte);
    }


    std::cerr << "PC out of bounds" << std::endl;
    exit(1);

}


void CPU::loop(Timer& timers, Memory& memory, Display& display){
    auto lastCpu = std::chrono::steady_clock::now();
    auto lastClock = std::chrono::steady_clock::now();
    while (true){
        auto now = std::chrono::steady_clock::now();
        auto cpuDt = std::chrono::duration<double>(now-lastCpu).count();
        auto clockDt = std::chrono::duration<double>(now-lastClock).count();

        while (cpuDt >= chip8::CPU_STEP){
            cycle(memory, display);
            cpuDt -= chip8::CPU_STEP;
            lastCpu += std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(chip8::CPU_STEP));
        }

        while (clockDt >= chip8::TIMER_STEP){
            if (timers.getDelayTimer() > 0){
                timers.decrementDelayTimer(); 
            }

            if (timers.getSoundTimer() > 0){
                timers.decrementSoundTimer();
            }

            clockDt -= chip8::TIMER_STEP;
            lastClock += std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(chip8::TIMER_STEP));
        }        

    }
}




void CPU::execute(u_int16_t instruction, Display& display, Memory& memory){
    u_int8_t opcode = (instruction & 0xF000) >> 12; //first nibble
    u_int8_t x = (instruction & 0x0F00) >> 8; //second nibble
    u_int8_t y = (instruction & 0x00F0) >> 4; //third nibble
    u_int8_t n = (instruction & 0x000F);  //fourth nibble
    u_int8_t nn = (instruction & 0x00FF); //second byte (third+fourth nibble)
    u_int16_t nnn = (instruction & 0x0FFF); //second, third, fourth, nibbles 


    switch (opcode){
        case 0:
            if (instruction == 0x00E0){
                clearScreen(display);
                break; 
            }

            else if (instruction == 0x00EE){
                returnFromSubroutine(memory);
                break;
            }
            break;

        case 1:
            jump(nnn);
            break;
        case 2:
            startSubroutine(nnn, memory);
            break;
        case 3:
            skipIfEqual(x, nn, memory);
            break;
        case 4:
            skipIfNotEqual(x,nn,memory);
            break;
        case 5:
            skipIfRegEqual(x,y,memory);
            break;
        case 6:
            setRegister(x, nn, memory);
            break;
        case 7:
            add (x, nn, memory);
            break;
        case 8:
            if (n == 0){
                setRegToReg(x,y,memory);
                break;
            }

            else if (n == 1){
                binaryOR(x,y,memory);
                break;
            }

            else if (n == 2){
                binaryAND(x, y, memory);
                break;
            }

            else if (n == 3){
                logicalXOR(x,y,memory);
                break;
            }

            else if (n == 4){
                addRegs(x,y,memory);
                break;
            }

            else if (n == 5){
                subtractYFromX(x,y,memory);
                break;
            }

            else if (n == 7){
                subtractXFromY(x,y,memory);
                break;
            }

            else if (n == 6){
                shiftRight(x,y,memory);
                break;
            }

            else if (n == 0xE){
                shiftLeft(x,y,memory);
                break;
            }
        break;

        case 9:
            skipIfRegNotEqual(x, y, memory);
            break;
        case 0xA:
            setIndexRegister(nnn, memory);
            break;
        case 0xD:
            dxyn(x, y, n, memory, display);
            break;
    }
}




void CPU::clearScreen(Display& display){
    for (int i = 0; i < 32; i ++){
        for (int j = 0; j < 64; j ++){
            display.turnPixelOff(i,j);
        }

    }
}

void CPU::jump(u_int16_t address){
    if (address >=0 && address < chip8::MEMORY_SIZE){
        pc = address;
    }
    else{
        std::cerr << "Error: Jumping to out of bounds address" << std::endl;
        exit(1);
    }
}



void CPU::setRegister(u_int8_t reg, u_int8_t value, Memory& memory){
    memory.setRegister(reg, value);
}



void CPU::add(u_int8_t reg, u_int8_t value, Memory& memory){
    memory.setRegister(reg, memory.getRegister(reg) + value);
}


void CPU::setIndexRegister(u_int16_t value, Memory& memory){
    memory.setIndexRegister(value);
}


void CPU::dxyn(u_int8_t xReg, u_int8_t yReg, u_int8_t height, Memory& memory, Display& display){
    u_int8_t xCoord = memory.getRegister(xReg) & (chip8::SCREEN_WIDTH-1);
    u_int8_t yCoord = memory.getRegister(yReg) & (chip8::SCREEN_HEIGHT-1);
    memory.setRegister(0xF, 0);

    for (u_int8_t row = 0; row < height; row ++){

        if ( memory.getIndexRegister() > chip8::MEMORY_SIZE-row-1){
            break;
        }

        u_int16_t spriteAddress = memory.getIndexRegister() + row;
        u_int8_t y = yCoord + row;


        if (y >= chip8::SCREEN_HEIGHT){
            break;
        }

        u_int8_t spriteByte = memory.readMemory(spriteAddress);

        for (u_int8_t bit = 0; bit < 8; bit ++){
            u_int8_t x = xCoord + bit;
            if (x >= chip8::SCREEN_WIDTH){
                break;
            }

            u_int8_t spritePixel = (spriteByte & (0x80 >> bit)) != 0;
            if (display.getPixel(y,x) == 1 && (spritePixel)){
                memory.setRegister(0xF, 1);
            }

            if ((display.getPixel(y,x) ^ spritePixel) == 0){
                display.turnPixelOff(y,x);
            }

            else{
                display.turnPixelOn(y,x);
            }
        }
    }



    display.drawScreen();
}

void CPU::cycle(Memory& memory, Display& display){
    u_int16_t instruction = fetch(memory);
    execute(instruction, display, memory);
}

void CPU::startSubroutine(uint16_t address, Memory& memory){
    if (address >= 0 && address < chip8::MEMORY_SIZE){
        memory.pushToStack(pc);
        pc = address;
    }

    else{
        std::cerr << "Error: Cannot start subroutine, address of bounds" << std::endl;
        exit(1);
    }
}

void CPU::returnFromSubroutine(Memory& memory){
    pc = memory.top();
}

void CPU::skipIfEqual(uint8_t reg, uint16_t value, Memory& memory){
    if (pc >= chip8::MEMORY_SIZE - 2){
        std::cerr << "Error: Cannot skip, address out of bounds" << std::endl;
        exit(1);
    }
    if (memory.getRegister(reg) == value){
        pc += 2;    
    }
}

void CPU::skipIfNotEqual(uint8_t reg, uint16_t value, Memory& memory){

    if (pc >= chip8::MEMORY_SIZE - 2){
        std::cerr << "Error: Cannot skip, address out of bounds" << std::endl;
        exit(1);
    }

    if (memory.getRegister(reg) != value){
        std::cout << "SKIPPING" << std::endl;
        pc += 2;
    }

    else{
        std::cout << "NOT SKIPPING" << std::endl;
    }
} 

void CPU::skipIfRegEqual(uint8_t xReg, uint8_t yReg, Memory& memory){
    if (pc  >= chip8::MEMORY_SIZE - 2){
        std::cerr << "Error: Cannot skip, address out of bounds" << std::endl;
        exit(1);
    }

    if (memory.getRegister(xReg) == memory.getRegister(yReg)){
        pc += 2;
    }
}

void CPU::skipIfRegNotEqual(uint8_t xReg, uint8_t yReg, Memory& memory){
    if (pc >= chip8::MEMORY_SIZE - 2){
        std::cerr << "Error: Cannot skip, address out of bounds" << std::endl;
        exit(1);
    }

    if (memory.getRegister(xReg) != memory.getRegister(yReg)){
        pc += 2;
    }
}

void CPU::setRegToReg(uint8_t xReg, uint8_t yReg, Memory& memory){
    memory.setRegister(xReg, memory.getRegister(yReg));
}

void CPU::binaryOR(uint8_t xReg, uint8_t yReg, Memory& memory){
    memory.setRegister(xReg, memory.getRegister(xReg) | memory.getRegister(yReg));
}

void CPU::binaryAND(uint8_t xReg, uint8_t yReg, Memory& memory){
    memory.setRegister(xReg, memory.getRegister(xReg) & memory.getRegister(yReg));
}

void CPU::logicalXOR(uint8_t xReg, uint8_t yReg, Memory& memory){
    memory.setRegister(xReg, memory.getRegister(xReg) ^ memory.getRegister(yReg));
}

void CPU::subtractYFromX(uint8_t xReg, uint8_t yReg, Memory& memory){
    if (memory.getRegister(xReg) >= memory.getRegister(yReg)){
        memory.setRegister(0xF, 1);
    }

    else{
        memory.setRegister(0xF, 0);
    }
    memory.setRegister(xReg, memory.getRegister(xReg) - memory.getRegister(yReg));
}


void CPU::subtractXFromY(uint8_t xReg, uint8_t yReg, Memory& memory){

    if (memory.getRegister(yReg) >= memory.getRegister(xReg)){
        memory.setRegister(0xF, 1);
    }

    else{
        memory.setRegister(0xF, 0);
    }


    memory.setRegister(xReg, memory.getRegister(yReg) - memory.getRegister(xReg));
}

void CPU::addRegs(uint8_t xReg, uint8_t yReg, Memory& memory){
    if (memory.getRegister(xReg) + memory.getRegister(yReg) > 255){
        memory.setRegister(0xF, 1);
    }

    else{
        memory.setRegister(0xF, 0);
    }

    memory.setRegister(xReg, memory.getRegister(xReg) + memory.getRegister(yReg));
}

void CPU::shiftRight(uint8_t xReg, uint8_t yReg, Memory& memory){
    memory.setRegister(0XF, memory.getRegister(xReg) & 1);
    memory.setRegister(xReg, memory.getRegister(xReg) >> 1);
}


void CPU::shiftLeft(uint8_t xReg, uint8_t yReg, Memory& memory){
    memory.setRegister(0XF, (memory.getRegister(xReg) & 0X80) >> 7);
    memory.setRegister(xReg, memory.getRegister(xReg) << 1);
}

