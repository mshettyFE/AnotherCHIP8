#include "Sound.h"

Oscillator::Oscillator(float rate, float volume) {
    current_step = 0,
    volume = volume,
    step_size = (2.0 * M_PI) / rate;
}

float Oscillator::next() {
  this->current_step += this->step_size;
  return sinf(this->current_step) * this->volume;
}