#ifndef MEMORY_H
#define MEMORY_H

#include <cinttypes>
#include <iostream>
#include "Constants.h"

class Memory{
private:
  uint8_t ram[MAX_RAM_SIZE];
  void write_character_set(unsigned char offset=0x050);
public:
    Memory();
    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);
    uint16_t read_instruction(uint16_t address) const;
// Debugging function
    void dump() const;
};

#endif