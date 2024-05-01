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
  interpreter.run_iterations(static_cast<int>(instructions_per_second/60), false, false);
  interpreter.update_window();
}

int main(int argc, char *argv[]){
  interpreter.load("../roms/tetris.ch8");
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(callback,0,1);
  emscripten_cancel_main_loop();
#else
    interpreter.load(argv[1]);
    interpreter.run_eternal(false,true);
#endif
  return 0;
}