#ifndef CPU_H
#define CPU_H

#include <stack>
#include <cinttypes>
#include <iostream>

// COWGOD!!!!
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1

const static  uint8_t max_stack_size=16;
const static  uint16_t I_MASK= 0x0FFF;

class CPU{
private:
// pseudo-registers
  uint16_t pc; // program counter
// stack
  std::deque<uint16_t> chip_stack;
// timers. Both decrement at a rate of 60 Hz.
  uint8_t sound;
  uint8_t delay;
// Public registers directly accessible by program
  uint8_t Vx[16]; // public 16-bit registers. Don't use register F
  uint16_t I; // Stores memory addresses. Only bottom 12 bits used
public:
  CPU();
  uint16_t get_pc() const;
  uint8_t get_sound() const;
  uint8_t get_delay() const;
  uint8_t get_Vx(unsigned int i) const;
  uint16_t get_I() const;

  void set_sound(uint8_t value);
  void set_delay(uint8_t value);

  void print() const;
};

#endif
