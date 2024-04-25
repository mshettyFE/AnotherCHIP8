#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>

int main(int argc, char *argv[]){

    CHIP8 interpreter(true);
    interpreter.load(argv[1]);
    interpreter.run_eternal(true);
  return 0;
}
