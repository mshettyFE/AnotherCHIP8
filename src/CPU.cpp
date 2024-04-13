#include "CPU.h"
#include <iostream>

CPU::CPU(){
  pc=0;
  sound=0;
  delay=0;
  for(int i=0; i< 16; ++i){
    Vx[i] = 0;
  }
  I=0;
}

uint16_t CPU::get_pc() const{return this->pc;}
uint8_t CPU::get_sound() const{return this->sound;}
uint8_t CPU::get_delay() const{return this->delay;}
uint8_t CPU::get_Vx(unsigned int i) const{
  if(i>=16){
    throw std::invalid_argument("Invalid Register");
  }
  return this->Vx[i];
}
uint16_t CPU::get_I() const{return this->I&&I_MASK;}

void CPU::set_sound(uint8_t value){
  this->sound = value;
}

void CPU::set_delay(uint8_t value){
  this->delay = value;
}

void CPU::print() const{
  std::cout << "PC: " << std::hex << std::showbase <<  get_pc() << std::endl;
  std::cout << "Sound: " << static_cast<unsigned int>(get_sound()) << std::endl;
  std::cout << "Delay: " << static_cast<unsigned int>(get_delay()) << std::endl;
  std::cout << "I: " << get_I() << std::endl;
  for(int i=0; i< 16; ++i){
    std::cout << "V" << std::hex << std::uppercase << i << " :" << std::dec<<  static_cast<unsigned int>(get_Vx(i)) << std::endl;
  }
}
