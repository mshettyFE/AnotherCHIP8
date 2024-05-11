#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/bind.h>
#endif
#include <stdio.h>
#include <iostream>

#include "CHIP8.h"

CHIP8 interpreter(true);

void callback(){
  interpreter.run_iterations(static_cast<int>(instructions_per_second/60.0) , false, false);
  interpreter.update_window();
}


void load_rom(std::string filename) {
  interpreter.set_run(false);
  interpreter.reset();
  interpreter.load(filename);
}

void set_pause(bool val){
  interpreter.set_pause(val);
}

bool get_paused(){
  return interpreter.get_pause();
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("load_rom", &load_rom);
  emscripten::function("set_pause", &set_pause);
  emscripten::function("get_paused", &get_paused);
}
#endif


int main(int argc, char *argv[]){
#ifdef __EMSCRIPTEN__
  interpreter.load("../data/roms/tetris.ch8");
  emscripten_set_main_loop(callback,0,1);
  emscripten_cancel_main_loop();
#else
    interpreter.load(argv[1]);
    interpreter.run_eternal(false,true);
#endif
  return 0;
}
