#ifndef MEMORY_H
#define MEMORY_H

#include <cinttypes>
#include <iostream>
#include "Constants.h"

class Memory{
private:
  uint8_t ram[MAX_RAM_SIZE]; // address space of CHIP8
  void write_character_set(unsigned char offset=CHAR_OFFSET); // write default sprite data to memory
public:
    Memory();
    
    uint8_t read(uint16_t address) const; // read value from memory
    void write(uint16_t address, uint8_t value); // write value to memory
    uint16_t read_machine_code(uint16_t address) const; // interpret the two bytes starting at variable address as a single 16 byte instruction

    void reset(); // clear memory, and use default data
    void dump() const; // crappy hexdump of memory contents
};

#endif