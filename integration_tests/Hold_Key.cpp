#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>

int main(int argc, char *argv[]){
  CHIP8 interpreter(true);
  interpreter.disp->test_checkers();
  interpreter.print();

  bool quit = false;
  uint16_t  a = 0;
  while (interpreter.get_running()) {
    interpreter.update_window();
    a = interpreter.get_last_keys();
    if(a){
        std::cout << a << std::endl;
    }
    else{
        a=0;
    }
  }
  return 0;
}