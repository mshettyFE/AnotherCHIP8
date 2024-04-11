#ifndef CHIP8_H
#define CHIP8_H

#include "CPU.h"
#include "Display.h"
#include "Memory.h"
#include "Keyboard.h"

// Good references
// [1] https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
// [2] http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

class CHIP8{
public:
    CPU cpu;
    Display disp;
    Memory mem;
    KeyPad keys;
    void print() const;
};

#endif