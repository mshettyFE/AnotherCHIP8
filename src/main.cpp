#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>

int main(int argc, char *argv[]){
  CHIP8 interpreter(true,false);
  interpreter.load("../tests/IBM.ch8");
  interpreter.mem->dump();
  interpreter.disassemble(true);
  SDL_Quit();
  return 0;
}
