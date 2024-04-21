#ifndef CPU_H
#define CPU_H

#include <stack>
#include <cinttypes>
#include <mutex>
#include <condition_variable>
#include <future>
#include <iostream>
#include <atomic>
#include <memory>
#include <chrono>
#include <array>

#include "Sound.h"

#include "Constants.h"
#include "Instruction.h"

// COWGOD!!!!
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1

void oscillator_callback(void *userdata, Uint8 *stream, int len);

constexpr int64_t get_clock_period(){
// To set the clock frequency at which the delay and sound timers get updated at
  double frequency = 60; // cycles/sec
  double period = 1.0/frequency; // sec/cycle
  auto as_duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::duration<double>(period)); // cast period in seconds to number of microseconds needed
  return as_duration.count(); //return the number of microseconds in 1/60 of a second.
}

class CPU{
private:
// pseudo-registers
  uint16_t pc; // program counter
// stack
  std::deque<uint16_t> chip_stack;
// timers. Both decrement at a rate of 60 Hz.
  std::atomic<uint8_t>  sound;
  std::atomic<uint8_t>  delay;

  bool threading_on;
// atomic variables to state wheather threads should continue to run
  std::atomic<int> sound_is_running;
  std::atomic<int> delay_is_running;
// futures to state whether threads have properly finished
  std::future<bool> sound_finished;
  std::future<bool> delay_finished;
// Public registers directly accessible by program
  std::array<uint8_t,16> Vx; // public 16-bit registers. Don't use register F
  uint16_t I; // Stores memory addresses. Only bottom 12 bits used
// decrement register atomically
  bool decrement_delay();
  bool decrement_sound();
  SDL_AudioDeviceID audio_device;
public:
  std::unique_ptr<Oscillator> osc = std::make_unique<Oscillator>(Oscillator(SAMPLE_RATE, INIT_VOLUME)); // sampler to play audio

  CPU(bool threading_on=true);
  ~CPU();

  uint16_t get_pc() const;
  uint8_t get_sound() const;
  uint8_t get_delay() const;
  uint8_t get_Vx(uint8_t i) const;
  uint8_t get_VF() const;
  uint16_t get_I() const;

  void set_sound(uint8_t value);
  void set_delay(uint8_t value);
  void set_Vx(uint8_t i,uint8_t value);
  void set_VF(bool is_set);
  void set_I(uint16_t value);

  bool get_sound_running();
  bool get_delay_running();

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
