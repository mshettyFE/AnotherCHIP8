#ifndef AUDIO_H
#define AUDIO_H

#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>

#include "Constants.h"

class SoundCard{
private:
  float current_step;
  float step_size;
  float volume;
public:
    bool debug;
    SoundCard(float rate, float vol);
    float next();
};

static SoundCard a4 = SoundCard(A4_OSC, 0.8f);

#endif