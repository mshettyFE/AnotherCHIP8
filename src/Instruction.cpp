#include "Instruction.h"

Instruction::Instruction(uint16_t machine_code){
// arguments from left to right are: highest 4 bits of high byte, lowest 4 bits of high byte, highest 4 bits of low byte, lowest 4 bits of low byte
// the opposite of build instruction
  this->hhb = uint8_t(machine_code >> 12 & 0x0F) ;
  this->lhb = uint8_t(machine_code >> 8 & 0x0F);
  this->hlb = uint8_t(machine_code >> 4 & 0x0F);
  this->llb = uint8_t( machine_code & 0x0F);
}

Instruction::Instruction(uint8_t hhb, uint8_t lhb, uint8_t hlb, uint8_t llb){
  this->hhb = hhb & 0x0F;
  this->lhb = lhb & 0x0F;
  this->hlb = hlb & 0x0F;
  this->llb = llb & 0x0F;
}

uint16_t Instruction::emit() const{
// We first select the lowest 4 bits of each input, since you can't really pass in 4 bits as an argument without some confusing schnanigans
// do the appropriate left shifting to each argument to get a properly formatted instruction
  return ((uint16_t)this->hhb <<12)|((uint16_t)this->lhb << 8)|((uint16_t)this->hlb<< 4)|((uint16_t)this->llb);
}
