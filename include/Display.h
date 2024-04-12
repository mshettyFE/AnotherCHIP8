#ifndef DISPLAY_H
#define DISPLAY_H

#include <array>
#include <cinttypes>
#include <SDL2/SDL.h>

const static  char dis_width = 64;
const static  char dis_height = 32;

class Display{
private:
  std::array<uint32_t,dis_height*dis_width> display;
  SDL_Window* window=nullptr;
  SDL_Renderer* renderer=nullptr;
  SDL_Surface* surface = nullptr;
  int get_index(unsigned int x, unsigned int y) const;
public: 
  Display();
  ~Display();
  void print() const;
  uint8_t operator()(unsigned int x, unsigned int y) const;
  void write(unsigned int x, unsigned int y, bool on);
  void test_checkers();
  void to_screen();
};

#endif