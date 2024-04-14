#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>

int main(int argc, char *argv[]){

  CHIP8 interpreter;
  interpreter.print();

  SDL_Event event;
  bool quit = false;
  while (!quit) {
      interpreter.disp.to_screen();
      while (SDL_PollEvent(&event)) {
          switch (event.type) {
            case SDL_QUIT:
              quit=true;
              break;
          }
          auto keys = interpreter.keys.which_keys_is_pressed();
          if(keys&ONE_PRESENT){
            SDL_PauseAudio(0);            
          }
      }
  }
  SDL_Quit();
  return 0;
}
