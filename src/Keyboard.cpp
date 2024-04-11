#include "Keyboard.h"
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <iomanip>

uint16_t KeyPad::which_keys_is_pressed(bool debug) const{
    SDL_PumpEvents();
    uint16_t output = 0;
    if(keyboardState[SDL_SCANCODE_1]){output  |= ONE_PRESENT; if(debug){std::cout << "ONE" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_2]){output  |= TWO_PRESENT; if(debug){std::cout << "TWO" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_3]){output  |= THREE_PRESENT; if(debug){std::cout << "THREE" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_4]){output  |= C_PRESENT; if(debug){std::cout << "C" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_Q]){output  |= FOUR_PRESENT; if(debug){std::cout << "FOUR" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_W]){output  |= FIVE_PRESENT; if(debug){std::cout << "FIVE" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_E]){output  |= SIX_PRESENT; if(debug){std::cout << "SIX" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_R]){output  |= D_PRESENT; if(debug){std::cout << "D" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_A]){output  |= SEVEN_PRESENT; if(debug){std::cout << "SEVEN" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_S]){output  |= EIGHT_PRESENT; if(debug){std::cout << "EIGHT" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_D]){output  |= NINE_PRESENT; if(debug){std::cout << "NINE" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_F]){output  |= E_PRESENT; if(debug){std::cout << "E" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_Z]){output  |= A_PRESENT; if(debug){std::cout << "A" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_X]){output  |= ZERO_PRESENT; if(debug){std::cout << "ZERO" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_C]){output  |= B_PRESENT; if(debug){std::cout << "B" << std::endl;}}
    if(keyboardState[SDL_SCANCODE_V]){output  |= F_PRESENT; if(debug){std::cout << "F" << std::endl;}}
    return output;
}

std::string decode_keys(uint16_t encrypted_keys){
    std::string output="";
    if(encrypted_keys & ONE_PRESENT){output += "1";}
    if(encrypted_keys & TWO_PRESENT){output += "2";}
    if(encrypted_keys & THREE_PRESENT){output += "3";}
    if(encrypted_keys & C_PRESENT){output += "C";}
    if(encrypted_keys & FOUR_PRESENT){output += "4";}
    if(encrypted_keys & FIVE_PRESENT){output += "5";}
    if(encrypted_keys & SIX_PRESENT){output += "6";}
    if(encrypted_keys & D_PRESENT){output += "D";}
    if(encrypted_keys & SEVEN_PRESENT){output += "7";}
    if(encrypted_keys & EIGHT_PRESENT){output += "8";}
    if(encrypted_keys & NINE_PRESENT){output += "9";}
    if(encrypted_keys & E_PRESENT){output += "E";}
    if(encrypted_keys & A_PRESENT){output += "A";}
    if(encrypted_keys & ZERO_PRESENT){output += "0";}
    if(encrypted_keys & B_PRESENT){output += "B";}
    if(encrypted_keys & F_PRESENT){output += "F";}
    return output;
}