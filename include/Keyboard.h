#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL.h>
#include "Constants.h"

class Keyboard{
private:
    uint16_t present_keys=0; // bitfield encoding the currently pressed keys
public:
    void update_state(bool debug=false); // Poll the keyboard, and update present_keys with the latest key presses
    uint16_t get_state(){return present_keys;}
};

#endif