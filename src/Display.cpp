#include "Display.h"
#include <iostream>
#include <stdexcept>

Display::Display(){
    for(int i=0; i< dis_height*dis_width; ++i){
        display[i] = 0;
    }
}


int Display::get_index(unsigned int  x, unsigned int  y) const{
    if(x>=64){    throw std::invalid_argument("x out of bounds");}
    if(y>=32){    throw std::invalid_argument("y out of bounds");}
    return y*64+x;
}

uint8_t Display::operator()(unsigned int x, unsigned int y) const{
    return display[get_index(x,y)];
}


void Display::print() const{
    for(char h=0; h<dis_height; h++){
        for(char w=0; w<dis_width; w++){
            std::cout << (unsigned int)display[get_index(w,h)] << " ";
        }
        std::cout << '\n';
    }
}

void Display::write(unsigned int x, unsigned int y){
    auto index = get_index(x,y);
    display[index] = 1;
}
