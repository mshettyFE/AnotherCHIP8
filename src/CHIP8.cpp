#include "CHIP8.h"
#include <fstream>
#include <vector>
#include <bitset>
#include <string>
#include <iterator>
#include <sstream> 

#define VERBOSE_CPU

CHIP8::CHIP8(bool visible){
    Display disp(visible);
}

void CHIP8::load(std::string filename){
    std::ifstream program(filename,std::ios::binary);
    if ( !(program.is_open()) ) {
        std::cout << "File " << __FILE__ << " Line " << __LINE__ << " " << std::endl;
        std::string err_msg = filename+" couldn't be opened!";
        throw std::runtime_error(err_msg);
    }
    program.unsetf(std::ios::skipws);
// grab length of file, in units of unsigned chars (ie. 1 byte)
    program.seekg (0, std::ios::end);
    int length = program.tellg();
    program.seekg (0, std::ios::beg);
    if(length > MAX_PROGRAM_SIZE){
        program.close();
        std::cout << "File " << __FILE__ << " Line " << __LINE__ << " " << std::endl;
        std::string err_msg = filename+" is too big!";
        throw std::runtime_error(err_msg);
    }
// CHIP-8 Programs must align with even addresses
    if(length%2!=0){
        program.close();
        std::cout << "File " << __FILE__ << " Line " << __LINE__ << " " << std::endl;
        std::string err_msg = filename+" is misaligned!";
        throw std::runtime_error(err_msg);
    }
// Assuming you got here, the program fits in the RAM of the CHIP-8. Copy the data into the temp buffer, then copy over to system
    std::vector<uint8_t> buffer;
    buffer.reserve(length);
    buffer.insert(buffer.begin(),std::istream_iterator<uint8_t>(program),std::istream_iterator<uint8_t>());
// iterate by two since we read two bytes at a time
    for(int i=0; i< length; i+=2){
        uint8_t first, second;
        if(is_big_endian()){
            first = buffer[i];
            second = buffer[i+1];
        }
        else{
            first = buffer[i+1];
            second = buffer[i];
        }
        this->mem.write(START+i, first);
        this->mem.write(START+i+1, second);
    }
    program.close();
    loaded = true;
}

void CHIP8::print() const{
    std::cout << "CPU" << std::endl;
    cpu.print();
    std::cout << "Display" << std::endl;
    disp.print();
    std::cout << "Memory" << std::endl;
    mem.dump();
}

std::string CHIP8::disassemble() {
    std::string output;
    for(int addr=START; addr<MAX_PROGRAM_SIZE; addr+=2){
        auto machine_code = this->mem.read_machine_code(addr);
        if(machine_code!=0){
            auto instruction = Instruction(machine_code);
            std::string assembly = decompile(instruction)+"\n";
            output.append(assembly);
        }
    }
    return output;
}

uint16_t CHIP8::fetch() const{
    if(loaded){
        return this->mem.read_machine_code(this->cpu.get_pc());
    }
    throw std::invalid_argument("No program loaded");
}

Instruction CHIP8::bundle(uint16_t machine_code) const{
    return Instruction(machine_code);
}

CHIP8::assembly_func CHIP8::decode(const Instruction& instr, std::string& out_msg, bool debug){
// COWGOD!!!!!!!!!!!!!!!!!
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1
    switch(instr.get_hhb()){
        case 0x0:
            // CLS
            auto mem_addr = instr.get_mem_addr();
            if(mem_addr==0x00E0){
                if(debug){out_msg = "CLS";}
                return &CHIP8::SYS;
            }
            // RET
            else if(mem_addr = 0x00EE){
                if(debug){out_msg = "RET";}
                return &CHIP8::RET;
            }
            // SYS addr
            else{
                if(debug){out_msg  = "SYS "+ hex_to_string<uint16_t>(instr.get_mem_addr());}
                return &CHIP8::SYS;
            }
            break;
        case 0x1:
            // Jump addr
            if(debug){out_msg  = "JP "+ hex_to_string<uint16_t>(instr.get_mem_addr());}
            return &CHIP8::JP_DIRECT;
            break;
        case 0x2:
            // CALL addr
            if(debug){out_msg  = "CALL "+ hex_to_string<uint16_t>(instr.get_mem_addr());}
            return &CHIP8::CALL_DIRECT;
            break;
        case 0x3:
            // SE Vx, byte
            if(debug){out_msg  = "SE "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " + hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::SE_DIRECT;
            break;
        case 0x4:
            // SNE Vx, byte
            if(debug){out_msg  = "SEN "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " + hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::SNE_DIRECT;
            break;
        case 0x5:
            // SE Vx, Vy
            if(debug){out_msg  = "SE "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " + hex_to_string<uint8_t>(instr.get_hlb())+" "+hex_to_string<uint8_t>(instr.get_llb());}
            return &CHIP8::SE_REG;
            break;
        case 0x6:
            // LD Vx, byte
            if(debug){out_msg  = "LD "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::LD_DIRECT;
            break;
        case 0x7:
            // ADD Vx, byte
            if(debug){out_msg  = "ADD "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::LD_DIRECT;
            break;
        case 0x8:
            break;
        case 0x9:
            break;
        case 0xA:
            break;
        case 0xB:
            break;
        case 0xC:
            break;
        case 0xD:
            break;
        case 0xE:
            break;
        case 0xF:
            break;
        default:
        throw std::invalid_argument(decoding_error(instr));
    }
}

std::string CHIP8::decoding_error(const Instruction& instr){
    std::string hex_instr = hex_to_string<uint16_t>(instr.get_machine_code());
    std::string output = "ERROR: ";
    output.append(hex_instr);
    output.append("\n");
    return output;
}

int CHIP8::execute(assembly_func fnc, const Instruction& instr){
    return (this->*fnc)(instr);
}

std::string CHIP8::decompile(const Instruction& instr) {
  std::string debug_msg;
  CHIP8::assembly_func func = decode(instr, debug_msg,true);
  return std::move(debug_msg);
}

bool is_big_endian(void){
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

int CHIP8::SYS(const Instruction& instr){}
int CHIP8::CLS(const Instruction& instr){}
int CHIP8::RET(const Instruction& instr){}
int CHIP8::JP_DIRECT(const Instruction& instr){}
int CHIP8::CALL_DIRECT(const Instruction& instr){}
int CHIP8::SE_DIRECT(const Instruction& instr){}
int CHIP8::SNE_DIRECT(const Instruction& instr){}
int CHIP8::SE_REG(const Instruction& instr){}
int CHIP8::LD_DIRECT(const Instruction& instr){}
int CHIP8::ADD_DIRECT(const Instruction& instr){}
int CHIP8::LD_REG(const Instruction& instr){}
int CHIP8::OR(const Instruction& instr){}
int CHIP8::AND(const Instruction& instr){}
int CHIP8::XOR(const Instruction& instr){}
int CHIP8::ADD(const Instruction& instr){}
int CHIP8::SUB(const Instruction& instr){}
int CHIP8::SHR(const Instruction& instr){}
int CHIP8::SUBN(const Instruction& instr){}
int CHIP8::SHL(const Instruction& instr){}
int CHIP8::SNE(const Instruction& instr){}
int CHIP8::LD_DIRECT_I(const Instruction& instr){}
int CHIP8::JP_OFFSET(const Instruction& instr){}
int CHIP8::RND(const Instruction& instr){}
int CHIP8::DRW(const Instruction& instr){}
int CHIP8::SKP(const Instruction& instr){}
int CHIP8::SKPN(const Instruction& instr){}
int CHIP8::LD_DELAY(const Instruction& instr){}
int CHIP8::LD_KEY(const Instruction& instr){}
int CHIP8::SET_DELAY(const Instruction& instr){}
int CHIP8::SET_SOUND(const Instruction& instr){}
int CHIP8::ADD_I(const Instruction& instr){}
int CHIP8::LD_SPRITE(const Instruction& instr){}
int CHIP8::STORE_BCD(const Instruction& instr){}
int CHIP8::LD_ARR(const Instruction& instr){}
int CHIP8::LOAD_BCD(const Instruction& instr){}