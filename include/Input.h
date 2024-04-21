#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include "Constants.h"

enum KEYS_MAPS{
    ONE_KEY=0,
    TWO_KEY=1,
    THREE_KEY=2,
    FOUR_KEY=3,
    Q_KEY=4,
    W_KEY=5,
    E_KEY=6,
    R_KEY=7,
    A_KEY=8,
    S_KEY=9,
    D_KEY=10,
    F_KEY=11,
    Z_KEY=12,
    X_KEY=13,
    C_KEY=14,
    V_KEY=15
};

enum KEYS{
    NOTHING       = 0,
    ONE_PRESENT   = 0b1000000000000000,
    TWO_PRESENT   = 0b0100000000000000,
    THREE_PRESENT = 0b0010000000000000,
    C_PRESENT     = 0b0001000000000000,
    FOUR_PRESENT  = 0b0000100000000000,
    FIVE_PRESENT  = 0b0000010000000000,
    SIX_PRESENT   = 0b0000001000000000,
    D_PRESENT     = 0b0000000100000000,
    SEVEN_PRESENT = 0b0000000010000000,
    EIGHT_PRESENT = 0b0000000001000000,
    NINE_PRESENT  = 0b0000000000100000,
    E_PRESENT     = 0b0000000000010000,
    A_PRESENT     = 0b0000000000001000,
    ZERO_PRESENT  = 0b0000000000000100,
    B_PRESENT     = 0b0000000000000010,
    F_PRESENT     = 0b0000000000000001,
};

class KeyPad{
private:
    std::unique_ptr<SDL_Event> event;
    bool quit=false;
public:
    KeyPad();
    uint16_t which_keys_is_pressed(bool debug=false);
    bool exit();
};


std::string decode_keys(uint16_t encrypted_keys);

#endif