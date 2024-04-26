#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cinttypes>
#include <string>
#include <chrono>

// CPU
static constexpr  uint8_t max_stack_size=16; // The stack can't be bigger than this number. 16 is standard.
static constexpr  uint16_t I_MASK= 0x0FFF; // Allows one to only select the bottom 3 nibbles of the I register
static constexpr uint8_t instruction_size = 2; // number of bytes that each instruction is

// Display
static constexpr char dis_width = 64; // width of screen
static constexpr char dis_height = 32; // height of screen
static constexpr char screen_scaling = 10; // scaling up of screen since 64x32 is small on modern displays
// Keyboard mapping
enum KEYS_MAPS{
    ZERO_KEY=0,
    ONE_KEY=1,
    TWO_KEY=2,
    THREE_KEY=3,
    FOUR_KEY=4,
    FIVE_KEY=5,
    SIX_KEY=6,
    SEVEN_KEY=7,
    EIGHT_KEY=8,
    NINE_KEY=9,
    A_KEY=11,
    B_KEY=12,
    C_KEY=13,
    D_KEY=14,
    E_KEY=15,
    F_KEY=15,
};

enum KEYS{
    NOTHING       = 0,
    F_PRESENT     = 0b1000000000000000,
    E_PRESENT     = 0b0100000000000000,
    D_PRESENT     = 0b0010000000000000,
    C_PRESENT     = 0b0001000000000000,
    B_PRESENT     = 0b0000100000000000,
    A_PRESENT     = 0b0000010000000000,
    NINE_PRESENT  = 0b0000001000000000,
    EIGHT_PRESENT = 0b0000000100000000,
    SEVEN_PRESENT = 0b0000000010000000,
    SIX_PRESENT   = 0b0000000001000000,
    FIVE_PRESENT  = 0b0000000000100000,
    FOUR_PRESENT  = 0b0000000000010000,
    THREE_PRESENT = 0b0000000000001000,
    TWO_PRESENT   = 0b0000000000000100,
    ONE_PRESENT   = 0b0000000000000010,
    ZERO_PRESENT  = 0b0000000000000001,
};


//Memory
static constexpr uint16_t MAX_RAM_SIZE = 4096; // Size of RAM. Include character set, program data, and normal RAM slots
static  constexpr uint16_t START = 0x200; // Starting location of all programs
static constexpr uint16_t MAX_PROGRAM_SIZE = 0xFFF-0x200; // MAX size of program allowed
static constexpr uint16_t CHAR_OFFSET = 0x050;

// Sound
static constexpr uint16_t SAMPLE_RATE = 44100; // Number of samples SDL plays per second
static constexpr uint16_t BUFFER_SIZE  = 4096; // Number of samples read in when SDL runs out of samples
static constexpr float INIT_VOLUME = 0.8f; // Constant initial volume.

// Speed
static constexpr int fps = 60;
static constexpr int instructions_per_second = 500; // "Clock Speed" of processor. In quotes b/c CHIP8 was never a hardware thing

static constexpr std::chrono::duration<double, std::nano> instr_time((1.0/static_cast<double>(instructions_per_second))*1E9);
static constexpr std::chrono::duration<double, std::nano> spf(1.0/fps*1E9);

#endif