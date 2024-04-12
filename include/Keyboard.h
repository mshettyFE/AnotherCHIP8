#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL.h>
#include <string>

static const std::string physical_keys[16] = {
    "1","2","3","C",
    "4","5","6","D",
    "7","8","9","E",
    "A","0","B","F"            
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
    const uint8_t* keyboardState = SDL_GetKeyboardState(NULL);
public:
    uint16_t which_keys_is_pressed(bool debug=false) const;
};

std::string decode_keys(uint16_t encrypted_keys);

#endif