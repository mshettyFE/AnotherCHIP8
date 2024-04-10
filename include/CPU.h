#ifndef CPU_H
#define CPU_H

#include <stack>
#include <iostream>

const uint8_t max_stack_size=16;

class CPU{
private:
// pseudo-registers
  uint16_t pc; // program counter
// stack
  std::deque<uint16_t> chip_stack;
// timers
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
  friend std::ostream& operator <<(std::ostream& output, const CPU& chip8);
};

#endif
