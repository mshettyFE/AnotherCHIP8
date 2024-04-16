#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cinttypes>

class Instruction{
private:
  uint8_t hhb; uint8_t lhb; uint8_t hlb; uint8_t llb;
public:
    Instruction(uint16_t instr);
    Instruction(uint8_t hhb, uint8_t lhb, uint8_t hlb, uint8_t llb);
    uint8_t get_hhb() const{return hhb;};
    uint8_t get_lhb() const{return lhb;};
    uint8_t get_hlb() const{return hlb;};
    uint8_t get_llb() const{return llb;};
    uint16_t emit() const;
};


#endif