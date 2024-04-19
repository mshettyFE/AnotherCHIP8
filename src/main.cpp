#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>

int main(int argc, char *argv[]){
  CHIP8 interpreter(true,false);
  interpreter.load("../tests/IBM.ch8");
  std::cout << interpreter.disassemble() << std::endl;
  SDL_Quit();
  return 0;
}
