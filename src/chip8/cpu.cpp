#include <iostream>
#include <random>
#include "memory.hpp"
#include "cpu.hpp"
#include "constants.hpp"
#include "keypad.hpp"

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


void CPU::loop(Timer& timer, Memory& memory, Display& display, Keypad& keypad){
    auto lastCpu = std::chrono::steady_clock::now();
    auto lastClock = std::chrono::steady_clock::now();
    while (true){
        auto now = std::chrono::steady_clock::now();
        auto cpuDt = std::chrono::duration<double>(now-lastCpu).count();
        auto clockDt = std::chrono::duration<double>(now-lastClock).count();

        while (cpuDt >= chip8::CPU_STEP){
            cycle(memory, display, keypad, timer);
            cpuDt -= chip8::CPU_STEP;
            lastCpu += std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(chip8::CPU_STEP));
        }

        while (clockDt >= chip8::TIMER_STEP){
            if (timer.getDelayTimer() > 0){
                timer.decrementDelayTimer(); 
            }

            if (timer.getSoundTimer() > 0){
                timer.decrementSoundTimer();
            }

            clockDt -= chip8::TIMER_STEP;
            lastClock += std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(chip8::TIMER_STEP));
        }        

    }
}




void CPU::execute(u_int16_t instruction, Display& display, Memory& memory, Keypad& keypad, Timer& timer){
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
        case 0xB:
            jumpWithOffset(nnn, memory);
            break;
        case 0xC:
            random(x, nn, memory);
            break;
        case 0xD:
            dxyn(x, y, n, memory, display);
            break;
        case 0xE:
            if (nn == 0x9E){
                skipIfPressed(x, keypad, memory);
                break;
            }

            else if (nn == 0xA1){
                skipIfNotPressed(x, keypad, memory);
                break;
            }
            break;
        case 0xF:
            if (nn == 0x07){
                getDelayTimer(x, timer, memory);
                break;
            }

            else if (nn == 0x15){
                setDelayTimer(x, timer, memory);
                break;
            }

            else if (nn == 0x18){
                setSoundTimer(x, timer, memory);
            }

            else if (nn == 0x0A){
                getKey(x, keypad, memory);
                break;
            }

            else if (nn == 0x29){
                fontCharacter(x, memory);
                break;
            }

            else if (nn == 0x33){
                binaryToDecimal(x, memory);
                break;
            }

            else if (nn == 0x55){
                storeMemory(x, memory);
                break;
            }

            else if (nn == 0x65){
                loadMemory(x, memory);
                break;
            }

            else if (nn == 0x1E){
                addToIndex(x, memory);
                break;
            }

            break;
        default:
            std::cerr << "ERROR: NO INSTRUCTION MATCHING THE OPCODE" << std::endl;
            exit(1);
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



    //use SDL layer instead of terminal
    //display.drawScreen();
}

void CPU::cycle(Memory& memory, Display& display, Keypad& keypad, Timer& timer){
    u_int16_t instruction = fetch(memory);



    execute(instruction, display, memory, keypad, timer);
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

void CPU::jumpWithOffset(uint16_t address, Memory& memory){
    if (address < chip8::MEMORY_SIZE - memory.getRegister(0)){
        pc = address + memory.getRegister(0);
    }

    else{
        std::cerr << "Error: PC out of range" << std::endl;
        exit(1);
    }

}

void CPU::random(uint8_t reg, uint8_t nn, Memory& memory){ //untested
    std::random_device r;
    std::mt19937 engine(r());
    std::uniform_int_distribution<uint8_t> dist(0,255);

    uint8_t randomNumber = dist(engine);
    memory.setRegister(reg, randomNumber & nn);
}

void CPU::skipIfPressed(uint8_t reg, Keypad& keypad, Memory& memory) { //untested
    if (keypad.isPressed(memory.getRegister(reg))){
        if (pc >= chip8::MEMORY_SIZE-2){
            std::cerr << "Error: PC out of range, cannot skip" << std::endl;
            exit(1);
        }

        else{
            pc += 2;
        }
    }
}


void CPU::skipIfNotPressed(uint8_t reg, Keypad& keypad, Memory& memory){ //untested
    if (!keypad.isPressed(memory.getRegister(reg))){
        if (pc >= chip8::MEMORY_SIZE-2){
            std::cerr << "Error: PC out of range, cannot skip" << std::endl;
            exit(1);
        }

        else{
            pc += 2;
        }
    } 
}

void CPU::getDelayTimer(uint8_t reg, Timer& timer, Memory& memory) const{ //untested
    memory.setRegister(reg, timer.getDelayTimer());
}

void CPU::setDelayTimer(uint8_t reg, Timer& timer, Memory& memory) const{ //untested
    timer.setDelayTimer(memory.getRegister(reg));
}

void CPU::setSoundTimer(uint8_t reg, Timer& timer, Memory& memory) const{ //untested
    timer.setSoundTimer(memory.getRegister(reg));
}

void CPU::addToIndex(uint8_t reg, Memory& memory) const{
    if (memory.getIndexRegister() >= 0xFFFF-memory.getRegister(reg)){
        std::cerr << "Error: Cannot add to index register, out of range" << std::endl;
        exit(1);
    }

    else{
        memory.setIndexRegister(memory.getIndexRegister() + memory.getRegister(reg));
    }
}

void CPU::getKey(uint8_t reg, Keypad& keypad, Memory& memory){
    uint8_t key = keypad.getKeyPressed();
    if (key == 0xFF){
        pc -= 2;
    }
    else{
        memory.setRegister(reg, key);
        std::cout << memory.getRegister(reg) << std::endl;
    }

}

void CPU::fontCharacter(uint8_t reg, Memory& memory){
    uint8_t character = memory.getRegister(reg);
    std::cout << "FONT CHARACTER: " <<  int(character) << std::endl;
    memory.setIndexRegister(chip8::FONT_START + (5 * character));
}

void CPU::binaryToDecimal(uint8_t reg, Memory& memory){
    //Extract the three decimal digits
    uint8_t num = memory.getRegister(reg);
    uint8_t firstDigit = num%10;
    num /= 10;
    uint8_t secondDigit = num%10;
    num /= 10;
    uint8_t thirdDigit = num;

    uint16_t address = memory.getIndexRegister();

    memory.writeMemory(address, thirdDigit);
    memory.writeMemory(address+1, secondDigit);
    memory.writeMemory(address+2, firstDigit);
}

void CPU::storeMemory(uint8_t reg, Memory& memory){
    for (uint8_t i = 0; i <= reg; i ++){
        memory.writeMemory(memory.getIndexRegister()+i, memory.getRegister(i));
    }
}

void CPU::loadMemory(uint8_t reg, Memory& memory){
    for (uint8_t i = 0; i <= reg; i ++){
        memory.setRegister(i, memory.readMemory(memory.getIndexRegister()+i));
        std::cout << "FOUND VALUE: " << std::hex << (int)(memory.getIndexRegister()+i) << std::endl;
    }
}

