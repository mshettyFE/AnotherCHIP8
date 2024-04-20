#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>

int main(int argc, char *argv[]){
  CHIP8 interpreter(true,true);
  interpreter.disp->test_checkers();
  interpreter.print();

  bool quit = false;
  uint16_t  a = 0;
  while (!quit) {
    interpreter.disp->to_screen();
    a = interpreter.keys->which_keys_is_pressed();
    if(a){
        std::cout << a << std::endl;
    }
    else{
        a=0;
    }
    if(interpreter.keys->exit()){
        quit = true;
    }
  }
  return 0;
}
