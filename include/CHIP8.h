#ifndef CHIP8_H
#define CHIP8_H

#include "CPU.h"
#include "Display.h"
#include "Memory.h"

class CHIP8{
public:
    CPU cpu;
    Display disp;
    Memory mem;
    void print() const;
};

#endif