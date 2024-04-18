#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>

int main(int argc, char *argv[]){
  bool debug = false;
  CHIP8 interpreter;
  interpreter.load("../tests/IBM.ch8");
  std::cout << interpreter.disassemble() << std::endl;
  SDL_Quit();
  return 0;
}
