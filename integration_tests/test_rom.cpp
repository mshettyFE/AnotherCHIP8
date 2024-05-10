#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>

int main(int argc, char *argv[]){
  CHIP8 interpreter(true);
  interpreter.load("../data/roms/test_rom.ch8");
  interpreter.run_eternal();
  return 0;
}
