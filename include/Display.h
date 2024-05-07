#ifndef DISPLAY_H
#define DISPLAY_H

#include <array>
#include <cinttypes>
#include <SDL2/SDL.h>
#include "Constants.h"

enum{PIXEL_OFF=0x0056B9, PIXEL_ON=0xFFD800};

class Display{
private:
  std::array<uint32_t,dis_height*dis_width> display; // pixel data
  // SDL stuff
  SDL_Window* window=nullptr;
  SDL_Renderer* renderer=nullptr;
  SDL_Texture* texture = nullptr;
  // map from x,y pixel coordinates to index in display array
  int get_index(unsigned int x, unsigned int y) const;
public: 
  Display(bool visible);
  ~Display();

  void print() const;
  void reset(); // hard reset display to blank screen

  uint32_t read(unsigned int x, unsigned int y) const; // read value at x,y coordinate
  bool write(unsigned int x, unsigned int y); // XOR the pixel at x,y
  void test_checkers(); // test pattern
  void to_screen(); // blit pixel data to SDL screen
};

#endif