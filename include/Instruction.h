#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cinttypes>
#include <string>

class Instruction{
// wrapper class around uint16_t to abstract away accessing certain nibbles and bytes and whatnot
private:
  uint8_t hhn; uint8_t lhn; uint8_t hln; uint8_t lln; // variable naming schema is as follows
  // hhn refers to the higher nibble of the higher byte
  // lhn refers to the lower nibble of the higher byte
  // hlh refers to the higher nibble of the lower byte
  // lln refers to the lower nibble of the lower byte
public:
  // can provide entire machine instruction, or individual nibbles
    Instruction(uint16_t instr);
    Instruction(uint8_t hhn, uint8_t lhn, uint8_t hln, uint8_t lln);
  // getter functions
    uint8_t get_hhn() const{return hhn;}
    uint8_t get_lhn() const{return lhn;}
    uint8_t get_hln() const{return hln;}
    uint8_t get_lln() const{return lln;}
    uint8_t get_lower_byte() const;
    uint8_t get_upper_byte() const;
    uint16_t get_mem_addr() const;
    uint16_t get_machine_code() const;
};

#endif