#include "CPU.h"
#include <iostream>

CPU::CPU(){
// pseudo-registers
  pc=0; // program counter
// timers
  sound=0;
  delay=0;
// Public registers directly accessible by program
  for(int i=0; i< 16; ++i){
    Vx[i] = 0;
  }
  I=0; // Stores memory addresses. Only bottom 12 bits used
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
uint16_t CPU::get_I() const{return this->I;}

std::ostream& operator <<(std::ostream& output, const CPU& chip8){
  output << "PC: " << std::hex << std::showbase <<  chip8.get_pc() << std::endl;
  output << "Sound: " << (unsigned int) chip8.get_sound() << std::endl;
  output << "Delay: " << (unsigned int) chip8.get_delay() << std::endl;
  output << "I: " << chip8.get_I() << std::endl;
  for(int i=0; i< 16; ++i){
    output << "V" << std::hex << std::uppercase << i << " :" << std::dec<<  (unsigned int)chip8.get_Vx(i) << std::endl;
  }
// TODO print out stack
  return output;
}
