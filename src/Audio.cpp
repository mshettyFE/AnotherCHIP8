#include "Audio.h"

void oscillator_callback(void *userdata, Uint8 *stream, int len) {
  float *fstream = (float *)stream;
  for (int i = 0; i < BUFFER_SIZE; i++) { // fill the buffer with the next BUFFER_SIZE samples
    float v = a4.next();
    fstream[i] = v;
  }
}

SoundCard::SoundCard(float rate, float vol, bool debug) {
    current_step = 0;
    step_size = static_cast<float>((2.0 * 3.14159265358979) / rate); // calculate the time step needed to correctly produce monoenergetic sound
    volume = vol;
    this->debug = debug;
}

float SoundCard::next() {
    this->current_step += this->step_size; // increment current step
    float ret = std::sin(this->current_step); // get sine value
    if(debug){
        std::cout << current_step << "\t" << ret << std::endl;
    }
    return ret * this->volume; // return scaled sine value
}