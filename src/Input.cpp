#include "Input.h"
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <iomanip>

KeyPad::KeyPad(){
    event = std::make_unique<SDL_Event>(SDL_Event());
}

bool KeyPad::poll_exit(){
    while(SDL_PollEvent(this->event.get()) != 0){
        switch(this->event->type){
            case SDL_QUIT:
                return true;
                break;
        }
    }
    return false;
}

uint16_t KeyPad::which_keys_is_pressed(bool debug){
    uint16_t output = 0;
    while(SDL_PollEvent(this->event.get()) != 0){
        switch(this->event->type){
            case SDL_KEYDOWN:
                if(event->key.keysym.sym == SDLK_1){output  |= ONE_PRESENT; if(debug){std::cout << "ONE" << std::endl;}}
                if(event->key.keysym.sym == SDLK_2){output  |= TWO_PRESENT; if(debug){std::cout << "TWO" << std::endl;}}
                if(event->key.keysym.sym == SDLK_3){output  |= THREE_PRESENT; if(debug){std::cout << "THREE" << std::endl;}}
                if(event->key.keysym.sym == SDLK_4){output  |= C_PRESENT; if(debug){std::cout << "C" << std::endl;}}
                if(event->key.keysym.sym == SDLK_q){output  |= FOUR_PRESENT; if(debug){std::cout << "FOUR" << std::endl;}}
                if(event->key.keysym.sym == SDLK_w){output  |= FIVE_PRESENT; if(debug){std::cout << "FIVE" << std::endl;}}
                if(event->key.keysym.sym == SDLK_e){output  |= SIX_PRESENT; if(debug){std::cout << "SIX" << std::endl;}}
                if(event->key.keysym.sym == SDLK_r){output  |= D_PRESENT; if(debug){std::cout << "D" << std::endl;}}
                if(event->key.keysym.sym == SDLK_a){output  |= SEVEN_PRESENT; if(debug){std::cout << "SEVEN" << std::endl;}}
                if(event->key.keysym.sym == SDLK_s){output  |= EIGHT_PRESENT; if(debug){std::cout << "EIGHT" << std::endl;}}
                if(event->key.keysym.sym == SDLK_d){output  |= NINE_PRESENT; if(debug){std::cout << "NINE" << std::endl;}}
                if(event->key.keysym.sym == SDLK_f){output  |= E_PRESENT; if(debug){std::cout << "E" << std::endl;}}
                if(event->key.keysym.sym == SDLK_z){output  |= A_PRESENT; if(debug){std::cout << "A" << std::endl;}}
                if(event->key.keysym.sym == SDLK_x){output  |= ZERO_PRESENT; if(debug){std::cout << "ZERO" << std::endl;}}
                if(event->key.keysym.sym == SDLK_c){output  |= B_PRESENT; if(debug){std::cout << "B" << std::endl;}}
                if(event->key.keysym.sym == SDLK_v){output  |= F_PRESENT; if(debug){std::cout << "F" << std::endl;}}
                break;
        }
    }
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

KEYS_MAPS parse_key(uint16_t encrypted_keys){
    if(encrypted_keys & ONE_PRESENT){return ONE_KEY;}
    if(encrypted_keys & TWO_PRESENT){return TWO_KEY;}
    if(encrypted_keys & THREE_PRESENT){return THREE_KEY;}
    if(encrypted_keys & C_PRESENT){return C_KEY;}
    if(encrypted_keys & FOUR_PRESENT){return FOUR_KEY;}
    if(encrypted_keys & FIVE_PRESENT){return FIVE_KEY;}
    if(encrypted_keys & SIX_PRESENT){return SIX_KEY;}
    if(encrypted_keys & D_PRESENT){return D_KEY;}
    if(encrypted_keys & SEVEN_PRESENT){return SEVEN_KEY;}
    if(encrypted_keys & EIGHT_PRESENT){return EIGHT_KEY;}
    if(encrypted_keys & NINE_PRESENT){return NINE_KEY;}
    if(encrypted_keys & E_PRESENT){return E_KEY;}
    if(encrypted_keys & A_PRESENT){return A_KEY;}
    if(encrypted_keys & ZERO_PRESENT){return ZERO_KEY;}
    if(encrypted_keys & B_PRESENT){return B_KEY;}
    if(encrypted_keys & F_PRESENT){return F_KEY;}
    return NO_KEY;
}
