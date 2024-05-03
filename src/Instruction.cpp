#include "Instruction.h"
#include <iostream>

Instruction::Instruction(uint16_t machine_code){
// arguments from left to right are: highest 4 bits of high byte, lowest 4 bits of high byte, highest 4 bits of low byte, lowest 4 bits of low byte
// the opposite of build instruction
  this->hhn = static_cast<uint8_t>(machine_code >> 12 & 0x0F) ;
  this->lhn = static_cast<uint8_t>(machine_code >> 8 & 0x0F);
  this->hln = static_cast<uint8_t>(machine_code >> 4 & 0x0F);
  this->lln = static_cast<uint8_t>( machine_code & 0x0F);
}

Instruction::Instruction(uint8_t hhn, uint8_t lhn, uint8_t hln, uint8_t lln){
  this->hhn = hhn & 0x0F;
  this->lhn = lhn & 0x0F;
  this->hln = hln & 0x0F;
  this->lln = lln & 0x0F;
}

uint8_t Instruction::get_lower_byte() const{
  return (hln << 4) | lln;
}

uint8_t Instruction::get_upper_byte() const{
  return (hhn << 4) | lhn;
}

uint16_t Instruction::get_mem_addr() const{
  return (static_cast<uint16_t>(lhn) << 8) | (static_cast<uint16_t>(hln) << 4) | (static_cast<uint16_t>(lln));
}

uint16_t Instruction::get_machine_code() const{
// We first select the lowest 4 bits of each input, since you can't really pass in 4 bits as an argument without some confusing schnanigans
// do the appropriate left shifting to each argument to get a properly formatted instruction
  return (static_cast<uint16_t>(hhn) <<12) | (static_cast<uint16_t>(lhn) << 8) | (static_cast<uint16_t>(hln)<< 4) | (static_cast<uint16_t>(lln));
}
