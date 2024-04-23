#ifndef CPU_H
#define CPU_H

#include <stack>
#include <cinttypes>
#include <iostream>
#include <memory>
#include <chrono>
#include <array>
#include <cmath>

#include "Sound.h"

#include "Constants.h"
#include "Instruction.h"

// COWGOD!!!!
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1

void oscillator_callback(void *userdata, Uint8 *stream, int len);

constexpr inline int64_t get_clock_period(){
  // returns number of computer system clock cycles that correspond to 1/60 seconds passing
  std::chrono::duration<double, std::ratio<1,60>> delta(1);
  return delta.count();
}

class CPU{
private:
// pseudo-registers
  uint16_t pc; // program counter
// stack
  std::deque<uint16_t> chip_stack;
// timers. Both decrement at a rate of 60 Hz.
  uint8_t  sound;
  uint8_t  delay;
  std::chrono::_V2::steady_clock::time_point last_update;
  // Public registers directly accessible by program
  std::array<uint8_t,16> Vx; // public 16-bit registers. Don't use register F
  uint16_t I; // Stores memory addresses. Only bottom 12 bits used
  SDL_AudioDeviceID audio_device;
public:
  std::unique_ptr<Oscillator> osc = std::make_unique<Oscillator>(Oscillator(SAMPLE_RATE, INIT_VOLUME)); // sampler to play audio

  CPU();
  ~CPU();

  uint16_t get_pc() const;
  uint8_t get_sound() const;
  uint8_t get_delay() const;
  uint8_t get_Vx(uint8_t i) const;
  uint8_t get_VF() const;
  uint16_t get_I() const;
  std::chrono::_V2::steady_clock::time_point get_last_update(){return last_update;}

  SDL_AudioDeviceID get_audio_device() const;

  void set_sound(uint8_t value);
  void set_delay(uint8_t value);
  void set_Vx(uint8_t i,uint8_t value);
  void set_VF(bool is_set);
  void set_I(uint16_t value);
  void set_last_update(){this->last_update = std::chrono::steady_clock::now();}

  void increment_pc();
  void decrement_pc();
  void set_pc(uint16_t value);

  void push_stack(uint16_t value);
  uint16_t pop_stack();
  uint16_t poke_stack(uint8_t value);
  uint8_t get_stack_size() const{return this->chip_stack.size();}

  void print() const;
  void reset();
};

#endif
