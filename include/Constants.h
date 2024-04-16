#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cinttypes>
#include <string>

// CPU
static constexpr  uint8_t max_stack_size=16;
static constexpr  uint16_t I_MASK= 0x0FFF;
// Display
static constexpr char dis_width = 64;
static constexpr char dis_height = 32;
// Keyboard
static const std::string physical_keys[16] = {
    "1","2","3","C",
    "4","5","6","D",
    "7","8","9","E",
    "A","0","B","F"            
};

//Memory
static constexpr int MAX_RAM_SIZE = 4096;
static  constexpr uint16_t START = 0x200;
static constexpr uint16_t MAX_PROGRAM_SIZE = 0xFFF-0x200;

// Sound
static constexpr uint16_t SAMPLE_RATE = 44100;
static constexpr uint16_t BUFFER_SIZE  = 4096;
static constexpr float INIT_VOLUME = 0.8f;



#endif