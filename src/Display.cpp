#include "Display.h"
#include "Constants.h"
#include <iostream>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <cstring>

Display::Display(bool visible){
    // initialize array with off color
    for(long unsigned int i=0; i< display.size(); i++){
        display[i] = PIXEL_OFF;
    }
    // Start SDL video
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) != 0){
        std::cout << SDL_GetError() << std::endl;
        throw std::invalid_argument("SDL_Init failed");
    }
    // Options to display window or not. Useful for unit tests when you don't want the window to phase in and out of existence
    if(visible){
        window = SDL_CreateWindow(
            "CHIP8",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            dis_width*screen_scaling,
            dis_height*screen_scaling,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );
    }
    else{
        window = SDL_CreateWindow(
            "CHIP8",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            dis_width*screen_scaling,
            dis_height*screen_scaling, SDL_WINDOW_HIDDEN
        );
    }
    // Initialize the SDL constructs
    if(window==nullptr){
        std::cout << SDL_GetError() << std::endl;
        throw std::invalid_argument("SDL_Window failed");
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer==nullptr){
        std::cout << SDL_GetError() << std::endl;
        throw std::invalid_argument("SDL_Renderer failed");
    }
    texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_STREAMING,dis_width,dis_height);
    if(texture ==nullptr){
        std::cout << SDL_GetError() << std::endl;
        std::invalid_argument("SDL_Texture failed");
    }
    // draw blank screen to SDL display
    to_screen();
}

Display::~Display(){
// clean up SDL constructs
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

int Display::get_index(unsigned int  x, unsigned int  y) const{
    if(x>=64){    throw std::invalid_argument("x out of bounds");}
    if(y>=32){    throw std::invalid_argument("y out of bounds");}
    return y*64+x;
}

uint32_t Display::read(unsigned int x, unsigned int y) const{
    return display[get_index(x,y)];
}


void Display::print() const{
    for(char h=0; h<dis_height; h++){
        for(char w=0; w<dis_width; w++){
            std::cout << std::hex << (unsigned int)display[get_index(w,h)] << " ";
        }
        std::cout << std::dec<< '\n';
    }
}

bool Display::write(unsigned int x, unsigned int y){
// spec demands that you XOR the selected pixel with PIXEL_ON
// Doing it with if-else statements since I couldn't figure out a nice way to do colors other than BLACK and WHITE with XOR
    auto index = get_index(x,y);
    bool set_VF = false;
    if(display[index]==PIXEL_ON){
        set_VF = true;
        display[index] = PIXEL_OFF;
    }
    else{
        display[index] = PIXEL_ON;
    }
    return set_VF;
}

void Display::reset(){
// set everything to 0 in display array
    for(unsigned long int i=0; i<dis_height*dis_width; ++i){
        display[i]=PIXEL_OFF;        
    }
}

void Display::test_checkers(){
// Write checker board pattern to display
    for(int i=0; i< dis_height; ++i){
        for(int j =0; j< dis_width; ++j){
            if((i+j)%2){
                write(i,j);
            }
        }
    }
}

void Display::to_screen(){
// Copy texture and expand to current window size
    SDL_Rect src, dest;
    src.x = 0;
    src.y = 0;
    src.w = dis_width;
    src.h = dis_height;
    int new_w, new_h;
    SDL_GetWindowSize(window,&new_w,&new_h);
    dest.w = (new_w< new_h) ? new_w : new_h;
    dest.h = dest.w;
    dest.x = (new_w-dest.w)/2;
    dest.y = (new_h-dest.h)/2;
//    std::cout << src.x << " " << src.y << " " << src.w << " " << src.h << std::endl;
//    std::cout << dest.x << " " << dest.y << " " << dest.w << " " << dest.h << std::endl << std::endl;
    if(SDL_UpdateTexture(texture, NULL, this->display.data(),dis_width*4)){
        std::cout << SDL_GetError() << std::endl;
        std::invalid_argument("SDL_Texture failed");
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,texture, &src, &dest);
    SDL_RenderPresent(renderer);
}