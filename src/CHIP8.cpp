#include "CHIP8.h"

void CHIP8::print() const{
    std::cout << "CPU" << std::endl;
    cpu.print();
    std::cout << "Display" << std::endl;
    disp.print();
    std::cout << "Memory" << std::endl;
    mem.dump();
}
