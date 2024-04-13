#include "CPU.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

CPU::CPU(){
  pc=0;
  sound = 0;
  delay=0;
  for(int i=0; i< 16; ++i){
    Vx[i] = 0;
  }
  I=0;
// Create and detach deamon thread that auto-decrements delay counter if non-zero
  std::thread delay_dec( [this] { this->decrement_delay(); } );
  delay_dec.detach();
// same for sound
  std::thread sound_dec( [this] { this->decrement_sound(); } );
  sound_dec.detach();
}

uint16_t CPU::get_pc() const{return this->pc;}
uint8_t CPU::get_sound() const{return this->sound.load();}
uint8_t CPU::get_delay() const{return this->delay.load();}
uint8_t CPU::get_Vx(unsigned int i) const{
  if(i>=16){
    throw std::invalid_argument("Invalid Register");
  }
  return this->Vx[i];
}
uint16_t CPU::get_I() const{return this->I&&I_MASK;}

void CPU::set_sound(uint8_t value){
  this->sound.store(value);
}

void CPU::set_delay(uint8_t value){
  this->delay.store(value);
}

void CPU::decrement_delay(){
  thread_local auto last_update = std::chrono::steady_clock::now();
  while(1){
// If you are already at 0, don't do anything
    if(this->get_delay()==0){
      last_update = std::chrono::steady_clock::now();
      continue;
    }
// Counter is non-zero. Check if elapsed time is greater than 
    auto now = std::chrono::steady_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::microseconds> (now-last_update);
    if(delta.count()>get_clock_period()){
      delay -= 1;
      last_update = std::chrono::steady_clock::now();
    }
  }
}

void CPU::decrement_sound(){ // identical to decrement delay, but should also feel sound into SDL2
  thread_local auto last_update = std::chrono::steady_clock::now();
  thread_local bool should_play_sound = false;
  while(1){
// If you are already at 0, check if you should be playing sound
    if(this->get_sound()==0){
      last_update = std::chrono::steady_clock::now();
      if(should_play_sound){
        should_play_sound = false;
// SDL hook to play sound goes here TODO
      }
      continue;
    }
    should_play_sound = true;
// Counter is non-zero. Check if elapsed time is greater than 
    auto now = std::chrono::steady_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::microseconds> (now-last_update);
    if(delta.count()>get_clock_period()){
      sound -= 1;
      last_update = std::chrono::steady_clock::now();
    }
  }
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
