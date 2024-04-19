#ifndef MEMORY_H
#define MEMORY_H

#include <cinttypes>
#include <iostream>
#include "Constants.h"

class Memory{
private:
  uint8_t ram[MAX_RAM_SIZE];
  void write_character_set(unsigned char offset=CHAR_OFFSET);
public:
    Memory();
    
    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);
    uint16_t read_machine_code(uint16_t address) const;

    void reset();
// Debugging function
    void dump() const;
};

#endif