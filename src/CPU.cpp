#include "CPU.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

CPU::CPU(){
  pc=0;
  sound = 0;
  delay=0;
  for(int i=0; i< 16; ++i){
    Vx[i] = 0;
  }
  I=0;
  SDL_AudioSpec desired_spec = {
      .freq = SAMPLE_RATE,
      .format = AUDIO_F32,
      .channels = 1,
      .samples = BUFFER_SIZE,
      .callback = oscillator_callback,
  };
  SDL_AudioSpec obtained_spec;

  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0){
      std::cout << SDL_GetError() << std::endl;
      throw std::invalid_argument("SDL_Init failed");
  }
  audio_device = SDL_OpenAudioDevice(NULL, 0, &desired_spec, &obtained_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
  if(audio_device==0){
      std::string message = "AHHHHH! Failed to open Audio Device: "+ std::string(SDL_GetError());
      throw std::invalid_argument(message);
	}
  SDL_PauseAudio(0);            
  SDL_PauseAudio(1);            
// Create and detach deamon thread that auto-decrements delay counter if non-zero
  std::thread delay_dec( [this] { this->decrement_delay(); } );
  delay_dec.detach();
// same for sound
  std::thread sound_dec( [this] { this->decrement_sound(); } );
  sound_dec.detach();
}

CPU::~CPU(){
  SDL_PauseAudio(0);
  SDL_CloseAudioDevice(audio_device);
}


uint16_t CPU::get_pc() const{return this->pc;}
uint8_t CPU::get_sound() const{return this->sound.load();}
uint8_t CPU::get_delay() const{return this->delay.load();}
uint8_t CPU::get_Vx(unsigned int i) const{
  if(i>=16){
    throw std::invalid_argument("Invalid Register");
  }
  return this->Vx[i];
}
uint16_t CPU::get_I() const{return this->I&&I_MASK;}

void CPU::set_sound(uint8_t value){
  this->sound.store(value);
}

void CPU::set_delay(uint8_t value){
  this->delay.store(value);
}

void CPU::decrement_delay(){
  thread_local auto last_update = std::chrono::steady_clock::now();
  while(1){
// If you are already at 0, don't do anything
    if(this->get_delay()==0){
      last_update = std::chrono::steady_clock::now();
      continue;
    }
// Counter is non-zero. Check if elapsed time is greater than 
    auto now = std::chrono::steady_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::microseconds> (now-last_update);
    if(delta.count()>get_clock_period()){
      delay -= 1;
      last_update = std::chrono::steady_clock::now();
    }
  }
}

void CPU::decrement_sound(){ // identical to decrement delay
  thread_local auto last_update = std::chrono::steady_clock::now();
  while(1){
// If you are already at 0, check if you should be playing sound
    if(this->get_sound()==0){
      SDL_PauseAudio(1);
      last_update = std::chrono::steady_clock::now();
      continue;
    }
    SDL_PauseAudio(0);
// Counter is non-zero. Check if elapsed time is greater than 
    auto now = std::chrono::steady_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::microseconds> (now-last_update);
    if(delta.count()>get_clock_period()){
      sound -= 1;
      last_update = std::chrono::steady_clock::now();
    }
  }
}

void CPU::print() const{
  std::cout << "PC: " << std::hex << std::showbase <<  get_pc() << std::endl;
  std::cout << "Sound: " << static_cast<unsigned int>(get_sound()) << std::endl;
  std::cout << "Delay: " << static_cast<unsigned int>(get_delay()) << std::endl;
  std::cout << "I: " << get_I() << std::endl;
  for(int i=0; i< 16; ++i){
    std::cout << "V" << std::hex << std::uppercase << i << " :" << std::dec<<  static_cast<unsigned int>(get_Vx(i)) << std::endl;
  }
  std::cout << audio_device << std::endl;
}

void oscillator_callback(void *userdata, Uint8 *stream, int len) {
  float *fstream = (float *)stream;
  for (int i = 0; i < BUFFER_SIZE; i++) {
    float v = osc.next();
    fstream[i] = v;
  }
}
