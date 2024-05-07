#include "Memory.h"
#include <iomanip>

Memory::Memory(){
    for(int i =0; i<MAX_RAM_SIZE; ++i){
        ram[i] = 0;
    }
    write_character_set();
}

void Memory::write_character_set(unsigned char offset){
// See [1] in CHIP8.h For where the heck this character set came from
    uint8_t vals[80] = {0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    for(int i=0; i< 80; ++i){
        ram[i+offset] = vals[i];
    }
}

void Memory::dump() const{
    for(int i=0; i<MAX_RAM_SIZE; i+=16){ // increment by 16 since we print 8 instructions per row, and each instruction consists of 2 bytes (8*2=16, and we are counting by bytes)
// Print the beginning address value
        std::cout << std::showbase<<  std::setw(5) << std::hex << std::uppercase << i << " ";
// Print the Hex values from the beginning offset
        for(int offset=0; offset<8; offset+=2){
            // construct instruction by bitshifting next two instructions
            uint16_t instruction = (static_cast<uint16_t>(ram[i+offset]) << 8) | static_cast<uint16_t>(ram[i+offset+1]);
            // formatting stuff to make it look pretty
            std::cout << std::hex << std::uppercase << std::noshowbase << std::setw(4) << std::setfill ( '0' ) << instruction << " ";
        }
        // Print the ASCII version
        std::cout <<std::setfill ( ' ' )<< "|" << std::dec;
        for(int offset=0; offset<16; offset++){
            auto val = ram[i+offset];
            if(std::isprint(val)){ // If you can print it as ASCII, do it, otherwise, just print .
                std::cout << val;
            }
            else{
                std::cout << ".";
            }
        }
        std::cout <<"|"; // terminate with pipe to make formatting look consistent
        std::cout << std::endl;
    }
}

uint8_t Memory::read(uint16_t address) const{
    if(address> MAX_RAM_SIZE){
        std::cout << "Trying to read address " << std::hex << address << std::dec << " Max RAM Size: " << std::hex << MAX_RAM_SIZE << std::dec << std::endl;
        throw std::invalid_argument("Address out of bounds. Can't read");
    }
    return ram[address];
}

uint16_t Memory::read_machine_code(uint16_t address) const{
// You can only read the lower 3 nibbles according to the spec. so use 0x0FFF mask to force address to comply
    address &= 0x0FFF;
    uint16_t first = ram[address];
    uint16_t second = ram[address+1];
    return (first << 8) | second;
}

void Memory::write(uint16_t address, uint8_t value){
    if(address> MAX_RAM_SIZE){
        throw std::invalid_argument("Address out of bounds. Can't write");
    }
    ram[address] = value;
}

void Memory::reset(){
    for(int i =0; i<MAX_RAM_SIZE; ++i){
        ram[i] = 0;
    }
    write_character_set();
}

