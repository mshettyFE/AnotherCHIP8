#include "Sound.h"
#include<cmath>
#include <iostream>

Oscillator::Oscillator(float rate, float volume) {
    current_step = 0;
    this->volume = volume;
    step_size = (2.0 * M_PI) / rate;
}

float Oscillator::next() {
  this->current_step += this->step_size;
  return std::sin(this->current_step) * this->volume;
}

void Oscillator::print(){
  std::cout << "Current Step: " << current_step << std::endl;
  std::cout << "Step Size: " << step_size << std::endl;
  std::cout << "Amplitude: " << volume << std::endl;  
}
