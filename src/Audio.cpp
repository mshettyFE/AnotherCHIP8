#include "Audio.h"

SoundCard::SoundCard(float rate, float vol) {
    current_step = 0;
    step_size = static_cast<float>((2.0 * 3.14159265358979) / rate);
    volume = vol;
}

float SoundCard::next() {
    float ret = std::sin(this->current_step);
    this->current_step += this->step_size;
    return ret * this->volume;
}