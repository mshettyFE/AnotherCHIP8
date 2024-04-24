#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>

int main(int argc, char *argv[]){

  if(argc != 2){
    std::cout << "Need to provide a ROM file" << std::endl;
    return 1;
  }

  CHIP8 interpreter(true);
  interpreter.load(argv[1]);
  interpreter.run_eternal();
  return 0;
}
