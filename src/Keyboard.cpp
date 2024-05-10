#include "Constants.h"
#include "Keyboard.h"

#include <iostream>

void Keyboard::update_state(bool debug){
    // in order to extract  keys, we OR the associated bit in present key if that key was pressed within the last 1/60th of a second
    present_keys = 0;
    SDL_PumpEvents();
    auto keyStates = SDL_GetKeyboardState(NULL);
    if(keyStates[SDL_SCANCODE_1]){present_keys  |= ONE_PRESENT; if(debug){std::cout << "ONE" << std::endl;}}
    if(keyStates[SDL_SCANCODE_2]){present_keys  |= TWO_PRESENT; if(debug){std::cout << "TWO" << std::endl;}}
    if(keyStates[SDL_SCANCODE_3]){present_keys  |= THREE_PRESENT; if(debug){std::cout << "THREE" << std::endl;}}
    if(keyStates[SDL_SCANCODE_4]){present_keys  |= C_PRESENT; if(debug){std::cout << "C" << std::endl;}}
    if(keyStates[SDL_SCANCODE_Q]){present_keys  |= FOUR_PRESENT; if(debug){std::cout << "FOUR" << std::endl;}}
    if(keyStates[SDL_SCANCODE_W]){present_keys  |= FIVE_PRESENT; if(debug){std::cout << "FIVE" << std::endl;}}
    if(keyStates[SDL_SCANCODE_E]){present_keys  |= SIX_PRESENT; if(debug){std::cout << "SIX" << std::endl;}}
    if(keyStates[SDL_SCANCODE_R]){present_keys  |= D_PRESENT; if(debug){std::cout << "D" << std::endl;}}
    if(keyStates[SDL_SCANCODE_A]){present_keys  |= SEVEN_PRESENT; if(debug){std::cout << "SEVEN" << std::endl;}}
    if(keyStates[SDL_SCANCODE_S]){present_keys  |= EIGHT_PRESENT; if(debug){std::cout << "EIGHT" << std::endl;}}
    if(keyStates[SDL_SCANCODE_D]){present_keys  |= NINE_PRESENT; if(debug){std::cout << "NINE" << std::endl;}}
    if(keyStates[SDL_SCANCODE_F]){present_keys  |= E_PRESENT; if(debug){std::cout << "E" << std::endl;}}
    if(keyStates[SDL_SCANCODE_Z]){present_keys  |= A_PRESENT; if(debug){std::cout << "A" << std::endl;}}
    if(keyStates[SDL_SCANCODE_X]){present_keys  |= ZERO_PRESENT; if(debug){std::cout << "ZERO" << std::endl;}}
    if(keyStates[SDL_SCANCODE_C]){present_keys  |= B_PRESENT; if(debug){std::cout << "B" << std::endl;}}
    if(keyStates[SDL_SCANCODE_V]){present_keys  |= F_PRESENT; if(debug){std::cout << "F" << std::endl;}}
    
    if(debug){
        std::cout << present_keys << std::endl;
    }
}