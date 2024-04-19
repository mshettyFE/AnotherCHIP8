#include "CHIP8.h"
#include <fstream>
#include <vector>
#include <bitset>
#include <string>
#include <iterator>
#include <sstream> 

#define VERBOSE_CPU

CHIP8::CHIP8(bool visible, bool threading){
    // problem here. cpu not gettin initialized with threading
    cpu = std::make_unique<CPU>(threading);
    disp = std::make_unique<Display>(visible);
    mem = std::make_unique<Memory>();
    keys = std::make_unique<KeyPad>();
}

bool CHIP8::is_big_endian(void){
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
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
        this->mem->write(START+i, first);
        this->mem->write(START+i+1, second);
    }
    program.close();
    loaded = true;
}

void CHIP8::print() const{
    std::cout << "CPU" << std::endl;
    cpu->print();
    std::cout << "Display" << std::endl;
    disp->print();
    std::cout << "Memory" << std::endl;
    mem->dump();
}

std::string CHIP8::disassemble() {
    std::string output;
    for(int addr=START; addr<MAX_PROGRAM_SIZE; addr+=2){
        auto machine_code = this->mem->read_machine_code(addr);
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
        return this->mem->read_machine_code(this->cpu->get_pc());
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
            switch(instr.get_mem_addr()){
                case 0x00E0:                
                    // CLS
                    if(debug){out_msg = "CLS";}
                    return &CHIP8::CLS;
                    break;
                case 0x00EE:
                    // RET
                    if(debug){out_msg = "RET";}
                    return &CHIP8::RET;
                    break;
                default:
                    // SYS addr
                    if(debug){out_msg  = "SYS "+ hex_to_string<uint16_t>(instr.get_mem_addr());}
                    return &CHIP8::SYS;
                    break;
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
            return &CHIP8::CALL;
            break;
        case 0x3:
            // SE Vx, byte
            if(debug){out_msg  = "SE "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +  hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::SE_DIRECT;
            break;
        case 0x4:
            // SNE Vx, byte
            if(debug){out_msg  = "SNE "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " + hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::SNE_DIRECT;
            break;
        case 0x5:
            switch(instr.get_llb()){
                case 0x0:
                    // SE Vx, Vy
                    if(debug){out_msg  = "SE "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " + hex_to_string<uint8_t>(instr.get_hlb());}
                    return &CHIP8::SE_REG;
                    break;
                default:
                    throw std::invalid_argument(decoding_error(instr));
                    break;
            }
        case 0x6:
            // LD Vx, byte
            if(debug){out_msg  = "LD "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::LD_DIRECT;
            break;
        case 0x7:
            // ADD Vx, byte
            if(debug){out_msg  = "ADD "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::ADD_DIRECT;
            break;
        case 0x8:
            switch(instr.get_llb()){
                case 0x0:
                    // LD Vx, Vy
                    if(debug){out_msg  = "LD "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_hlb());}
                    return &CHIP8::LD_REG;
                    break;
                case 0x1:
                    // OR Vx, Vy
                    if(debug){out_msg  = "OR "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_hlb());}
                    return &CHIP8::OR;
                    break;
                case 0x2:
                    // AND Vx, Vy
                    if(debug){out_msg  = "AND "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_hlb());}
                    return &CHIP8::AND;
                    break;
                case 0x3:
                    // XOR Vx, Vy
                    if(debug){out_msg  = "XOR "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_hlb());}
                    return &CHIP8::XOR;
                    break;
                case 0x4:
                    // ADD Vx, Vy
                    if(debug){out_msg  = "ADD "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_hlb());}
                    return &CHIP8::ADD;
                    break;
                case 0x5:
                    // SUB Vx, Vy
                    if(debug){out_msg  = "SUB "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_hlb());}
                    return &CHIP8::SUB;
                    break;
                case 0x6:
                    // SHR Vx, Vy
                    if(debug){out_msg  = "SHR "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_hlb());}
                    return &CHIP8::SHR;
                    break;
                case 0x7:
                    // SUBN Vx, Vy
                    if(debug){out_msg  = "SUBN "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_hlb());}
                    return &CHIP8::SUBN;
                    break;
                case 0xE:
                    // SHL Vx {, Vy}
                    if(debug){out_msg  = "SHL "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_hlb());}
                    return &CHIP8::SHL;
                    break;
                default:
                   throw std::invalid_argument(decoding_error(instr));
                    break;
            }
            break;
        case 0x9:
            switch(instr.get_llb()){
                case 0x0:
                    // SNE Vx, Vy
                    if(debug){out_msg  = "SNE "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_hlb());}
                    return &CHIP8::SNE;
                    break;
                default:
                   throw std::invalid_argument(decoding_error(instr));
                    break;
            }
        case 0xA:
            // LD I, addr
            if(debug){out_msg  = "LD I,"+ instr.get_mem_addr();}
            return &CHIP8::LD_DIRECT_I;
            break;
        case 0xB:
            // JP V0, addr
            if(debug){out_msg  = "JP V0"+ instr.get_mem_addr();}
            return &CHIP8::JP_OFFSET;
            break;
        case 0xC:
            // RND Vx, byte
            if(debug){out_msg  = "RND "+ hex_to_string<uint8_t>(instr.get_lhb())+" " +hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::RND;
            break;
        case 0xD:
            // DRW Vx, Vy, nibble
            if(debug){out_msg  = "DRW "+ hex_to_string<uint8_t>(instr.get_lhb())+ " " +hex_to_string<uint8_t>(instr.get_hlb())+" "+hex_to_string<uint8_t>(instr.get_llb());}
            return &CHIP8::DRW;
            break;
        case 0xE:
            switch(instr.get_lower_byte()){
                case 0x9E:
                    // SKP Vx
                    if(debug){out_msg  = "SKP "+ hex_to_string<uint8_t>(instr.get_lhb());}
                    return &CHIP8::SKP;
                    break;
                case 0xA1:
                    // SKNP Vx
                    if(debug){out_msg  = "SKNP "+ hex_to_string<uint8_t>(instr.get_lhb());}
                    return &CHIP8::SKNP;
                    break;
                default:
                    throw std::invalid_argument(decoding_error(instr));
                    break;
            }
            break;
        case 0xF:
            switch (instr.get_lower_byte()){
                case 0x07:
                    // LD Vx, DT
                    if(debug){out_msg  = "LD "+ hex_to_string<uint8_t>(instr.get_lhb())+" DT";}
                    return &CHIP8::LD_DIRECT;
                    break;
                case 0x0A:
                    // LD Vx, K
                    if(debug){out_msg  = "LD "+ hex_to_string<uint8_t>(instr.get_lhb())+" K";}
                    return &CHIP8::LD_KEY;
                    break;
                case 0x15:
                    // LD DT, Vx
                    if(debug){out_msg  = "LD DT "+ hex_to_string<uint8_t>(instr.get_lhb());}
                    return &CHIP8::SET_DELAY;
                    break;
                case 0x18:
                    // LD DT, Vx
                    if(debug){out_msg  = "LD ST "+ hex_to_string<uint8_t>(instr.get_lhb());}
                    return &CHIP8::SET_SOUND;
                    break;
                case 0x1E:
                    // ADD I, Vx
                    if(debug){out_msg  = "ADD I, "+ hex_to_string<uint8_t>(instr.get_lhb());}
                    return &CHIP8::ADD_I;
                    break;
                case 0x29:
                    // LD F, Vx
                    if(debug){out_msg  = "LD F, "+ hex_to_string<uint8_t>(instr.get_lhb());}
                    return &CHIP8::LD_SPRITE;
                    break;
                case 0x33:
                    // LD B, Vx
                    if(debug){out_msg  = "LD B, "+ hex_to_string<uint8_t>(instr.get_lhb());}
                    return &CHIP8::STORE_BCD;
                    break;
                case 0x055:
                    // LD [I], Vx
                    if(debug){out_msg  = "LD [I], "+ hex_to_string<uint8_t>(instr.get_lhb());}
                    return &CHIP8::LD_ARR;
                    break;
                case 0x65:
                    // LD Vx, [I]
                    if(debug){out_msg  = "LD "+ hex_to_string<uint8_t>(instr.get_lhb())+ " [I]";}
                    return &CHIP8::LOAD_BCD;
                    break;
                
                default:
                    throw std::invalid_argument(decoding_error(instr));
                    break;
            }
            break;
        default:
            throw std::invalid_argument(decoding_error(instr));
            break;
    }
}

std::string CHIP8::decoding_error(const Instruction& instr){
    std::string hex_instr = hex_to_string<uint16_t>(instr.get_machine_code());
    std::string output = "ERROR: ";
    output.append(hex_instr);
    output.append("\n");
    return output;
}

void CHIP8::execute(assembly_func fnc, const Instruction& instr){
    (this->*fnc)(instr);
    return;
}

std::string CHIP8::decompile(const Instruction& instr) {
  std::string debug_msg;
  decode(instr, debug_msg,true);
  return debug_msg;
}

std::string CHIP8::test_instruction(const Instruction& instr){
    std::string debug_msg;
    CHIP8::assembly_func func = decode(instr, debug_msg,true);
    execute(func,instr);
    return debug_msg;
}

void CHIP8::reset(){
    cpu->reset();
    disp->reset();
    mem->reset();
}

void CHIP8::SYS(const Instruction& instr){
// do nothing but increment PC
    cpu->increment_pc();
}

void CHIP8::CLS(const Instruction& instr){
    disp->reset();
    cpu->increment_pc();
}

void CHIP8::RET(const Instruction& instr){
    auto new_pc = this->cpu->pop_stack();
    this->cpu->set_pc(new_pc);
}

void CHIP8::JP_DIRECT(const Instruction& instr){
    auto mem_addr = instr.get_mem_addr();
    if(mem_addr%2==1){
        throw std::invalid_argument("Jumping to odd address");
    }
    this->cpu->set_pc(mem_addr);
}

void CHIP8::CALL(const Instruction& instr){
    this->cpu->push_stack(this->cpu->get_pc());
    this->cpu->set_pc(instr.get_mem_addr());
}

void CHIP8::SE_DIRECT(const Instruction& instr){
    if(this->cpu->get_Vx(instr.get_lhb()) == instr.get_lower_byte()){
        this->cpu->increment_pc();
    }
    this->cpu->increment_pc();
}

void CHIP8::SNE_DIRECT(const Instruction& instr){
    if(this->cpu->get_Vx(instr.get_lhb()) != instr.get_lower_byte()){
        this->cpu->increment_pc();
    }
    this->cpu->increment_pc();
}

void CHIP8::SE_REG(const Instruction& instr){
    if(this->cpu->get_Vx(instr.get_hlb()) == this->cpu->get_Vx(instr.get_lhb())){
        this->cpu->increment_pc();
    }
    this->cpu->increment_pc();
}

void CHIP8::LD_DIRECT(const Instruction& instr){
    this->cpu->set_Vx(instr.get_lhb(),instr.get_lower_byte());
}

void CHIP8::ADD_DIRECT(const Instruction& instr){
    auto reg= instr.get_lhb();
    auto result = this->cpu->get_Vx(reg)+instr.get_lower_byte();
    this->cpu->set_Vx(reg,result);
}

void CHIP8::LD_REG(const Instruction& instr){
    this->cpu->set_Vx(instr.get_lhb(),this->cpu->get_Vx(instr.get_hlb()));
}

void CHIP8::OR(const Instruction& instr){
    auto result = this->cpu->get_Vx(instr.get_lhb()) | this->cpu->get_Vx(instr.get_hlb());
    this->cpu->set_Vx(instr.get_lhb(),result);
}

void CHIP8::AND(const Instruction& instr){
    auto result = this->cpu->get_Vx(instr.get_lhb()) & this->cpu->get_Vx(instr.get_hlb());
    this->cpu->set_Vx(instr.get_lhb(),result);
}

void CHIP8::XOR(const Instruction& instr){
    auto result = this->cpu->get_Vx(instr.get_lhb()) ^ this->cpu->get_Vx(instr.get_hlb());
    this->cpu->set_Vx(instr.get_lhb(),result);
}

void CHIP8::ADD(const Instruction& instr){

    uint16_t result = static_cast<uint16_t>(this->cpu->get_Vx(instr.get_lhb())) + static_cast<uint16_t>(this->cpu->get_Vx(instr.get_hlb()));
    this->cpu->set_Vx(instr.get_lhb(),result & 0x00FF);
    if(result & 0xFF00){
        this->cpu->set_VF(1);
    }
}

void CHIP8::SUB(const Instruction& instr){}
void CHIP8::SHR(const Instruction& instr){}
void CHIP8::SUBN(const Instruction& instr){}
void CHIP8::SHL(const Instruction& instr){}
void CHIP8::SNE(const Instruction& instr){}
void CHIP8::LD_DIRECT_I(const Instruction& instr){}
void CHIP8::JP_OFFSET(const Instruction& instr){}
void CHIP8::RND(const Instruction& instr){}
void CHIP8::DRW(const Instruction& instr){}
void CHIP8::SKP(const Instruction& instr){}
void CHIP8::SKNP(const Instruction& instr){}
void CHIP8::LD_DELAY(const Instruction& instr){}
void CHIP8::LD_KEY(const Instruction& instr){}
void CHIP8::SET_DELAY(const Instruction& instr){}
void CHIP8::SET_SOUND(const Instruction& instr){}
void CHIP8::ADD_I(const Instruction& instr){}
void CHIP8::LD_SPRITE(const Instruction& instr){}
void CHIP8::STORE_BCD(const Instruction& instr){}
void CHIP8::LD_ARR(const Instruction& instr){}
void CHIP8::LOAD_BCD(const Instruction& instr){}