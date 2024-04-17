#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cinttypes>
#include <string>

class Instruction{
private:
  uint8_t hhb; uint8_t lhb; uint8_t hlb; uint8_t llb;
public:
    Instruction(uint16_t instr);
    Instruction(uint8_t hhb, uint8_t lhb, uint8_t hlb, uint8_t llb);
    uint8_t get_hhb() const{return hhb;}
    uint8_t get_lhb() const{return lhb;}
    uint8_t get_hlb() const{return hlb;}
    uint8_t get_llb() const{return llb;}
    uint8_t get_lower_byte() const;
    uint8_t get_upper_byte() const;
    uint16_t get_mem_addr() const;
    uint16_t get_machine_code() const;
};

#endif