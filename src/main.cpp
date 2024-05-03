#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include <stdio.h>
#include <iostream>

#include "CHIP8.h"

CHIP8 interpreter(true);

void callback(){
  interpreter.run_iterations(3 , false, false);
  interpreter.update_window();
}

int main(int argc, char *argv[]){
#ifdef __EMSCRIPTEN__
  interpreter.load("../roms/Breakout.ch8");
  emscripten_set_main_loop(callback,0,1);
  emscripten_cancel_main_loop();
#else
    interpreter.load(argv[1]);
//    interpreter.run_eternal(false,true);
    interpreter.run_iterations(1000,false,false);
#endif
  return 0;
}