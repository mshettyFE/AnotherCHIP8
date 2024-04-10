#include "Memory.h"
#include <iomanip>

Memory::Memory(){
    for(int i =0; i<MAX_RAM_SIZE; ++i){
        ram[i] = 0;
    }
}

void Memory::dump() const{
    for(int i=0; i<MAX_RAM_SIZE; i+=16){
// Print the beginning address value
        std::cout << std::showbase<<  std::setw(5) << std::hex << std::uppercase << i << " ";
// Print the Hex values from the beginning offset
        for(int offset=0; offset<16; offset++){
            std::cout << std::hex << std::uppercase << std::noshowbase << std::setw(2) << std::setfill ( '0' ) << static_cast<unsigned int>(ram[i+offset]) << " ";
        }
// Print the ASCII version.. if you can
        std::cout <<std::setfill ( ' ' )<< "|" << std::dec;
        for(int offset=0; offset<16; offset++){
            auto val = ram[i+offset];
            if(std::isprint(val)){
                std::cout << val;
            }
            else{
                std::cout << ".";
            }
        }
        std::cout <<"|";
        std::cout << std::endl;
    }
}
