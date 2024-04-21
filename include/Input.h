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
    C_KEY=3,
    FOUR_KEY=4,
    FIVE_KEY=5,
    SIX_KEY=6,
    D_KEY=7,
    SEVEN_KEY=8,
    EIGHT_KEY=9,
    NINE_KEY=10,
    E_KEY=11,
    A_KEY=12,
    ZERO_KEY=13,
    B_KEY=14,
    F_KEY=15,
    NO_KEY=16
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
    bool poll_exit();
};

std::string decode_keys(uint16_t encrypted_keys);
KEYS_MAPS parse_key(uint16_t encrypted_keys);

#endif