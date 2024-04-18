#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cinttypes>
#include <string>

// CPU
static constexpr  uint8_t max_stack_size=16; // The stack can't be bigger than this number. 16 is standard.
static constexpr  uint16_t I_MASK= 0x0FFF; // Allows one to only select the bottom 3 nibbles of the I register
// Display
static constexpr char dis_width = 64; // width of screen
static constexpr char dis_height = 32; // height of screen
static constexpr char screen_scaling = 10; // scaling up of screen since 64x32 is small on modern displays
// Keyboard mapping
static const std::string physical_keys[16] = {
    "1","2","3","C",
    "4","5","6","D",
    "7","8","9","E",
    "A","0","B","F"            
};

//Memory
static constexpr int MAX_RAM_SIZE = 4096; // Size of RAM. Include character set, program data, and normal RAM slots
static  constexpr uint16_t START = 0x200; // Starting location of all programs
static constexpr uint16_t MAX_PROGRAM_SIZE = 0xFFF-0x200; // MAX size of program allowed

// Sound
static constexpr uint16_t SAMPLE_RATE = 44100; // Number of samples SDL plays per second
static constexpr uint16_t BUFFER_SIZE  = 4096; // Number of samples read in when SDL runs out of samples
static constexpr float INIT_VOLUME = 0.8f; // Constant initial volume.

// Speed
static constexpr int instructions_per_second = 600; // "Clock Speed" of processor. In quotes b/c CHIP8 was never a hardware thing, so 600 is so that games are playable

#endif