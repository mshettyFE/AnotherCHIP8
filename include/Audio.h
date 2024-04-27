#ifndef AUDIO_H
#define AUDIO_H

#include <cmath>
#include <iostream>

#include "Constants.h"

class SoundCard{
private:
  float current_step;
  float step_size;
  float volume;
public:
    SoundCard(float rate, float vol);
    float next();
};

static SoundCard a4 = SoundCard(A4_OSC, 0.8f);

#endif