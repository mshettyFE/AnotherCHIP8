#ifndef MEMORY_H
#define MEMORY_H

#include <cinttypes>
#include <iostream>

const static int MAX_RAM_SIZE = 4096;
const static  uint16_t START = 0x200;

class Memory{
private:
  uint8_t ram[MAX_RAM_SIZE];
public:
    Memory();
    void dump() const;
};

#endif