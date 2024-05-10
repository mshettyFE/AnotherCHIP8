#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>

int main(int argc, char *argv[]){
  CHIP8 interpreter(true);
  interpreter.load("../data/roms/IBM.ch8");
  interpreter.disassemble();
  SDL_Quit();
  return 0;
}
