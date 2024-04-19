#include "Instruction.h"

Instruction::Instruction(uint16_t machine_code){
// arguments from left to right are: highest 4 bits of high byte, lowest 4 bits of high byte, highest 4 bits of low byte, lowest 4 bits of low byte
// the opposite of build instruction
  this->hhb = static_cast<uint8_t>(machine_code >> 12 & 0x0F) ;
  this->lhb = static_cast<uint8_t>(machine_code >> 8 & 0x0F);
  this->hlb = static_cast<uint8_t>(machine_code >> 4 & 0x0F);
  this->llb = static_cast<uint8_t>( machine_code & 0x0F);
}

Instruction::Instruction(uint8_t hhb, uint8_t lhb, uint8_t hlb, uint8_t llb){
  this->hhb = hhb & 0x0F;
  this->lhb = lhb & 0x0F;
  this->hlb = hlb & 0x0F;
  this->llb = llb & 0x0F;
}

uint8_t Instruction::get_lower_byte() const{
  return (hlb << 4) | llb;
}

uint8_t Instruction::get_upper_byte() const{
  return (hhb << 4) | lhb;
}

uint16_t Instruction::get_mem_addr() const{
  return (static_cast<uint16_t>(lhb) << 8) | (static_cast<uint16_t>(hlb) << 4) | (static_cast<uint16_t>(llb));
}

uint16_t Instruction::get_machine_code() const{
// We first select the lowest 4 bits of each input, since you can't really pass in 4 bits as an argument without some confusing schnanigans
// do the appropriate left shifting to each argument to get a properly formatted instruction
  return (static_cast<uint16_t>(hhb) <<12) | (static_cast<uint16_t>(lhb) << 8) | (static_cast<uint16_t>(hlb)<< 4) | (static_cast<uint16_t>(llb));
}
