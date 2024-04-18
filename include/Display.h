#ifndef DISPLAY_H
#define DISPLAY_H

#include <array>
#include <cinttypes>
#include <SDL2/SDL.h>
#include "Constants.h"

enum{BLACK=0, WHITE=0xFFFFFFFF};

class Display{
private:
  std::array<uint32_t,dis_height*dis_width> display;
  SDL_Window* window=nullptr;
  SDL_Renderer* renderer=nullptr;
  SDL_Surface* surface = nullptr;
  int get_index(unsigned int x, unsigned int y) const;
public: 
  Display(bool visible=true);
  ~Display();
  void print() const;
  void clear();
  uint32_t operator()(unsigned int x, unsigned int y) const; // read value at x,y coordinate
  void write(unsigned int x, unsigned int y);
  void test_checkers();
  void to_screen();
};

#endif