#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>

int main(int argc, char *argv[]){

  if(argc != 2){
    CHIP8 interpreter(true);
    interpreter.load("../roms/tetris.ch8");
    interpreter.run_eternal();
  }
  else{
    CHIP8 interpreter(true);
    interpreter.load(argv[1]);
    interpreter.run_eternal();
  }

  return 0;
}
