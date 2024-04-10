#ifndef DISPLAY_H
#define DISPLAY_H

#include <array>
#include <cinttypes>

const static  char dis_width = 64;
const static  char dis_height = 32;

class Display{
private:
  std::array<uint8_t,dis_height*dis_width> display;
  int get_index(unsigned int x, unsigned int y) const;
public:
  Display();
  void print() const;
  uint8_t operator()(unsigned int x, unsigned int y) const;
  void write(unsigned int x, unsigned int y);
};

#endif