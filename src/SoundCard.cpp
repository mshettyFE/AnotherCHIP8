#include "SoundCard.h"

void oscillator_callback(void *userdata, Uint8 *stream, int len) {
  float *fstream = (float *)stream;
  for (int i = 0; i < BUFFER_SIZE; i++) { // fill the buffer with the next BUFFER_SIZE samples
    float v =  static_cast<SoundCard*>(userdata)->next();
    fstream[i] = v;
  }
}

SoundCard::SoundCard(float rate, float vol, bool debug) {
    current_step = 0;
    step_size = static_cast<float>((2.0 * 3.14159265358979) / rate); // calculate the time step needed to correctly produce monoenergetic sound
    volume = vol;
    this->debug = debug;

    SDL_AudioSpec desired_spec = {
        .freq = SAMPLE_RATE,
        .format = AUDIO_F32,
        .channels = 1,
        .samples = BUFFER_SIZE,
        .callback = oscillator_callback,
        .userdata = this,
    };
    SDL_AudioSpec obtained_spec;
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) != 0){
        std::cout <<  SDL_GetError() << std::endl;
        throw std::invalid_argument("SDL_Init failed");
    }
    audio_device_id = SDL_OpenAudioDevice(NULL, 0, &desired_spec, &obtained_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if(audio_device_id==0){
        std::string message = "AHHHHH! Failed to open Audio Device: "+ std::string(SDL_GetError());
        throw std::invalid_argument(message);
    }
    SDL_PauseAudioDevice(audio_device_id,0);
    SDL_PauseAudioDevice(audio_device_id,1);

}

SoundCard::~SoundCard(){
  SDL_CloseAudioDevice(audio_device_id);
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

float SoundCard::next() {
    this->current_step += this->step_size; // increment current step
    float ret = std::sin(this->current_step); // get sine value
    if(debug){
        std::cout << current_step << "\t" << ret << std::endl;
    }
    return ret * this->volume; // return scaled sine value
}

void SoundCard::set_pause(int x){
  SDL_PauseAudioDevice(audio_device_id,x);
}

SDL_AudioDeviceID SoundCard::get_device_id() const{
  return this->audio_device_id;
}
