#ifndef CPU_H
#define CPU_H

#include <stack>
#include <cinttypes>
#include <iostream>
#include <memory>
#include <chrono>
#include <array>
#include <cmath>

#include "Constants.h"
#include "Instruction.h"

// COWGOD!!!!
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1

class CPU{
private:
  uint16_t pc; // program counter
  std::deque<uint16_t> chip_stack;// stack
  // timers. Both decrement at a rate of 60 Hz.
  uint8_t  sound;
  uint8_t  delay;
  std::chrono::steady_clock::time_point last_update; // variable to know how much to decrement sound and delay based on real time passage
  // Public registers directly accessible by program
  std::array<uint8_t,16> Vx; // public 16-bit registers
  uint16_t I; // Stores memory addresses. Only bottom 12 bits used
public:

  CPU();

  uint16_t get_pc() const;
  uint8_t get_sound() const;
  uint8_t get_delay() const;
  uint8_t get_Vx(uint8_t i) const;
  uint8_t get_VF() const;
  uint16_t get_I() const;
  std::chrono::steady_clock::time_point get_last_update();

  void set_sound(uint8_t value);
  void set_delay(uint8_t value);
  void set_Vx(uint8_t i,uint8_t value);
  void set_VF(bool is_set);
  void set_I(uint16_t value);
  void set_last_update();

  // access control for PC
  void increment_pc();
  void decrement_pc();
  void set_pc(uint16_t value);

  // access control for stack
  void push_stack(uint16_t value);
  uint16_t pop_stack();
  uint16_t poke_stack(uint8_t value);
  uint8_t get_stack_size() const{return this->chip_stack.size();}

  // utility
  void print() const;
  void reset();
};

#endif
