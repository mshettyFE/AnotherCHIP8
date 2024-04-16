#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>
#include "Constants.h"

class Oscillator{
private:
  float current_step;
  float step_size;
  float volume;
public:    
    Oscillator(float rate, float volume);
    float next();
    void print();
};

#endif