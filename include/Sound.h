#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>

static constexpr uint16_t SAMPLE_RATE = 44100;
static constexpr uint16_t BUFFER_SIZE  = 4096;
static constexpr float INIT_VOLUME = 0.8f;

class Oscillator{
private:
  float current_step;
  float step_size;
  float volume;
public:    
    Oscillator(float rate, float volume);
    float next();
};

#endif