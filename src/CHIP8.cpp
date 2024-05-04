#include "CHIP8.h"
#include <fstream>
#include <vector>
#include <bitset>
#include <string>
#include <iterator>
#include <sstream>
#include <random>
#include <iostream>

CHIP8::CHIP8(bool visible){
    // unique pointers so that I don't have to manage the memory of these
    disp = std::make_unique<Display>(visible);
    cpu = std::make_unique<CPU>();
    mem = std::make_unique<Memory>();
    keys = std::make_unique<Keyboard>();
    sound = std::make_unique<SoundCard>();
    last_instruction_time =std::chrono::steady_clock::now();
    last_keyboard_time = last_instruction_time;
}

void CHIP8::load(std::string filename){
// assumes that filename is big_endian
    std::ifstream program(filename,std::ios::binary);
    if ( !(program.is_open()) ) {
        std::cout << "File " << __FILE__ << " Line " << __LINE__ << " " << std::endl;
        std::string err_msg = filename+" couldn't be opened!";
        throw std::runtime_error(err_msg);
    }
    program.unsetf(std::ios::skipws); // don't ignore PIXEL_ONspace, since that might be valuable data
    program.seekg (0, std::ios::end); // grab length of file, in units of unsigned chars (ie. 1 byte)
    int length = program.tellg();
    program.seekg (0, std::ios::beg); // go back to the start of the file
    if(length > MAX_PROGRAM_SIZE){ // Can't load the program if it's too big
        program.close();
        std::cout << "File " << __FILE__ << " Line " << __LINE__ << " " << std::endl;
        std::string err_msg = filename+" is too big!";
        throw std::runtime_error(err_msg);
    }
    // Assuming you got here, the program fits in the RAM of the CHIP-8. Copy the data into the temp buffer, then copy over to system
    std::vector<uint8_t> buffer;
    buffer.reserve(length);
    buffer.insert(buffer.begin(),std::istream_iterator<uint8_t>(program),std::istream_iterator<uint8_t>());
    // Copy buffer memory to CHIP8 internal memory. iterate by two since we read two bytes at a time
    for(int i=0; i< length; i+=2){
        uint8_t first, second;
        first = buffer[i];
        second = buffer[i+1];
        this->mem->write(START+i, first);
        this->mem->write(START+i+1, second);
    }
// cleanup
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

std::string CHIP8::disassemble(bool raw) {
    std::string output;
    std::cout << "ADDR INST ASSM" << std::endl;
    for(int addr=START; addr<MAX_PROGRAM_SIZE; addr+=2){
        uint16_t machine_code = this->mem->read_machine_code(addr); // get 
        if(machine_code!=0){
            auto instruction = Instruction(machine_code);
            std::string assembly;
            try{
                assembly = decompile(instruction);
            }
            catch(std::invalid_argument &e){
                // NOTE: as per https://www.reddit.com/r/EmuDev/comments/iel9cg/chip8_distinguish_opcode_and_sprite_data/
                // You can't differentiate between sprite_data and opcodes just from raw binary file. So "UNKNOWN CODE" might just be sprite data. You don't know a priori!
                std::cout <<"UNKONWN CODE: " <<  hex_to_string<uint16_t>(addr) << " " << hex_to_string<uint16_t>(machine_code) << std::endl;
                continue;
            }
            if(raw){// print out the machine code in hex as well
                std::cout << hex_to_string<uint16_t>(addr) << " " << hex_to_string<uint16_t>(machine_code) << ": " << assembly << std::endl;
            }
            else{ // just print the assembly
                std::cout << hex_to_string<uint16_t>(addr) << ": " << assembly << std::endl;
            }
            output.append(assembly);
        }
    }
    return output;
}

uint16_t CHIP8::fetch() const{// try reading the current program counter. If you can't, throw error
    if(loaded){
        return this->mem->read_machine_code(this->cpu->get_pc());
    }
    throw std::invalid_argument("No program loaded");
}

CHIP8::assembly_func CHIP8::decode(const Instruction& instr, std::string& out_msg, bool debug){
//http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
    switch(instr.get_hhn()){
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
            if(debug){out_msg  = "SE "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +  hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::SE_DIRECT;
            break;
        case 0x4:
            // SNE Vx, byte
            if(debug){out_msg  = "SNE "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " + hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::SNE_DIRECT;
            break;
        case 0x5:
            switch(instr.get_lln()){
                case 0x0:
                    // SE Vx, Vy
                    if(debug){out_msg  = "SE "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " + hex_to_string<uint8_t>(instr.get_hln());}
                    return &CHIP8::SE_REG;
                    break;
                default:
                    throw std::invalid_argument(decoding_error(instr));
                    break;
            }
        case 0x6:
            // LD Vx, byte
            if(debug){out_msg  = "LD "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::LD_DIRECT;
            break;
        case 0x7:
            // ADD Vx, byte
            if(debug){out_msg  = "ADD "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::ADD_DIRECT;
            break;
        case 0x8:
            switch(instr.get_lln()){
                case 0x0:
                    // LD Vx, Vy
                    if(debug){out_msg  = "LD "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_hln());}
                    return &CHIP8::LD_REG;
                    break;
                case 0x1:
                    // OR Vx, Vy
                    if(debug){out_msg  = "OR "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_hln());}
                    return &CHIP8::OR;
                    break;
                case 0x2:
                    // AND Vx, Vy
                    if(debug){out_msg  = "AND "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_hln());}
                    return &CHIP8::AND;
                    break;
                case 0x3:
                    // XOR Vx, Vy
                    if(debug){out_msg  = "XOR "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_hln());}
                    return &CHIP8::XOR;
                    break;
                case 0x4:
                    // ADD Vx, Vy
                    if(debug){out_msg  = "ADD "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_hln());}
                    return &CHIP8::ADD;
                    break;
                case 0x5:
                    // SUB Vx, Vy
                    if(debug){out_msg  = "SUB "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_hln());}
                    return &CHIP8::SUB;
                    break;
                case 0x6:
                    // SHR Vx, Vy
                    if(debug){out_msg  = "SHR "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_hln());}
                    return &CHIP8::SHR;
                    break;
                case 0x7:
                    // SUBN Vx, Vy
                    if(debug){out_msg  = "SUBN "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_hln());}
                    return &CHIP8::SUBN;
                    break;
                case 0xE:
                    // SHL Vx {, Vy}
                    if(debug){out_msg  = "SHL "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_hln());}
                    return &CHIP8::SHL;
                    break;
                default:
                   throw std::invalid_argument(decoding_error(instr));
                    break;
            }
            break;
        case 0x9:
            switch(instr.get_lln()){
                case 0x0:
                    // SNE Vx, Vy
                    if(debug){out_msg  = "SNE "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_hln());}
                    return &CHIP8::SNE;
                    break;
                default:
                   throw std::invalid_argument(decoding_error(instr));
                    break;
            }
        case 0xA:
            // LD I, addr
            if(debug){out_msg  = "LD I, "+ hex_to_string<uint16_t>(instr.get_mem_addr());}
            return &CHIP8::LD_DIRECT_I;
            break;
        case 0xB:
            // JP V0, addr
            if(debug){out_msg  = "JP V0 "+ hex_to_string<uint8_t>(instr.get_mem_addr());}
            return &CHIP8::JP_OFFSET;
            break;
        case 0xC:
            // RND Vx, byte
            if(debug){out_msg  = "RND "+ hex_to_string<uint8_t>(instr.get_lhn())+" " +hex_to_string<uint8_t>(instr.get_lower_byte());}
            return &CHIP8::RND;
            break;
        case 0xD:
            // DRW Vx, Vy, nibble
            if(debug){out_msg  = "DRW "+ hex_to_string<uint8_t>(instr.get_lhn())+ " " +hex_to_string<uint8_t>(instr.get_hln())+" "+hex_to_string<uint8_t>(instr.get_lln());}
            return &CHIP8::DRW;
            break;
        case 0xE:
            switch(instr.get_lower_byte()){
                case 0x9E:
                    // SKP Vx
                    if(debug){out_msg  = "SKP "+ hex_to_string<uint8_t>(instr.get_lhn());}
                    return &CHIP8::SKP;
                    break;
                case 0xA1:
                    // SKNP Vx
                    if(debug){out_msg  = "SKNP "+ hex_to_string<uint8_t>(instr.get_lhn());}
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
                    if(debug){out_msg  = "LD "+ hex_to_string<uint8_t>(instr.get_lhn())+" DT";}
                    return &CHIP8::LD_DELAY;
                    break;
                case 0x0A:
                    // LD Vx, K
                    if(debug){out_msg  = "LD "+ hex_to_string<uint8_t>(instr.get_lhn())+" K";}
                    return &CHIP8::LD_KEY;
                    break;
                case 0x15:
                    // LD DT, Vx
                    if(debug){out_msg  = "LD DT "+ hex_to_string<uint8_t>(instr.get_lhn());}
                    return &CHIP8::SET_DELAY;
                    break;
                case 0x18:
                    // LD DT, Vx
                    if(debug){out_msg  = "LD ST "+ hex_to_string<uint8_t>(instr.get_lhn());}
                    return &CHIP8::SET_SOUND;
                    break;
                case 0x1E:
                    // ADD I, Vx
                    if(debug){out_msg  = "ADD I, "+ hex_to_string<uint8_t>(instr.get_lhn());}
                    return &CHIP8::ADD_I;
                    break;
                case 0x29:
                    // LD F, Vx
                    if(debug){out_msg  = "LD F, "+ hex_to_string<uint8_t>(instr.get_lhn());}
                    return &CHIP8::LD_SPRITE;
                    break;
                case 0x33:
                    // LD B, Vx
                    if(debug){out_msg  = "LD B, "+ hex_to_string<uint8_t>(instr.get_lhn());}
                    return &CHIP8::STORE_BCD;
                    break;
                case 0x055:
                    // LD [I], Vx
                    if(debug){out_msg  = "LD [I], "+ hex_to_string<uint8_t>(instr.get_lhn());}
                    return &CHIP8::STR_ARR;
                    break;
                case 0x65:
                    // LD Vx, [I]
                    if(debug){out_msg  = "LD "+ hex_to_string<uint8_t>(instr.get_lhn())+ " [I]";}
                    return &CHIP8::LD_ARR;
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
    this->cpu->increment_pc();
    (this->*fnc)(instr);
    this->tick_clock(); // Sync frame rate, and do other time-sensitive events
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
    loaded=true;
    update_draw = false;
    running = true;
    
    last_instruction_time = std::chrono::steady_clock::now();
    last_keyboard_time = last_instruction_time;

    event = SDL_Event();
    cpu->reset();
    disp->reset();
    mem->reset();
}

void CHIP8::run_eternal(bool verbose, bool display){
    if(!this->loaded){
        throw std::invalid_argument("No ROM loaded!");
    }
    std::string out_msg;
    while(this->running){
        if(display){
            this->update_window();
        }
        auto binary = fetch();
        Instruction instr(binary);
        auto cur_func = decode(instr,out_msg,verbose);
        if(verbose){
            std::cout << std::hex <<this->cpu->get_pc() << " " << hex_to_string<uint16_t>(binary) << std::dec  << " : " << out_msg<<std::endl;
        }
        execute(cur_func,instr);
    }
}

void CHIP8::run_iterations(unsigned int count, bool verbose, bool display){
    while(count >0){
        if(!this->loaded){
            throw std::invalid_argument("No ROM loaded!");
        }
        std::string out_msg;
        if(display){
            this->update_window();
        }
        auto binary = fetch();
        Instruction instr(binary);
        auto cur_func = decode(instr,out_msg,verbose);
        if(verbose){
            std::cout << std::hex <<this->cpu->get_pc() << " " << hex_to_string<uint16_t>(binary) << std::dec  << " : " << out_msg<<std::endl;
        }
        execute(cur_func,instr);
        count--;
    }
}

void CHIP8::tick_clock(){
    auto sound  = this->cpu->get_sound();
    auto delay  = this->cpu->get_delay();
    auto elaped_real_time = std::chrono::steady_clock::now()-this->cpu->get_last_update();
    uint8_t decrement = elaped_real_time.count()/spf.count();
    if(decrement){ // if decrement is non-zero, enough time has passed that we need to update registers
       if(sound > 0){
            this->sound->set_pause(0);// make sure that audio is playing of sound register is non-zero
            if(decrement > sound){
                this->cpu->set_sound(0);
            }
            else{
                this->cpu->set_sound(sound-decrement);
            }
        }
        if(sound==0){
            this->sound->set_pause(1); // pause audio if sound is 0
        }
        if(delay >0){
            if(decrement > delay){
                this->cpu->set_delay(0);
            }
            else{
                this->cpu->set_delay(delay-decrement);
            }
        }
        this->cpu->set_last_update(); // don't forget to update last recorded time stamp!
    }
    auto elapsed_instruction_time = std::chrono::steady_clock::now()-this->last_instruction_time;
    if(elapsed_instruction_time < instr_time){
        auto wait_time = std::chrono::duration_cast<std::chrono::milliseconds>(instr_time-elapsed_instruction_time).count();
        SDL_Delay(wait_time  ); // sleep if the instruction finished too early
    }
    this->last_instruction_time = std::chrono::steady_clock::now();
    auto elapsed_key_time = std::chrono::steady_clock::now()-this->last_keyboard_time;
    // get latest keyboard inputs if enough time has passed
    if(elapsed_key_time > spf){
        this->keys->update_state();
        this->last_keyboard_time = std::chrono::steady_clock::now();
    }
}

void CHIP8::update_window(bool debug){
    while(SDL_PollEvent(&event) != 0){
        switch(event.type){
            case SDL_QUIT:
                this->running = false;
                break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                this->update_draw  = true;
            }
        }
    }
    if(this->update_draw){
        this->disp->to_screen();
        this->update_draw =false; // Don't want to keep updating screen if no additional draw calls were made
    }
}

void CHIP8::SYS(const Instruction& instr){
// do nothing but increment PC
}

void CHIP8::CLS(const Instruction& instr){
// Clear screen
    disp->reset();
}

void CHIP8::RET(const Instruction& instr){
// return from function call, and set pc appropriately
    auto new_pc = this->cpu->pop_stack();
    this->cpu->set_pc(new_pc);
}

void CHIP8::JP_DIRECT(const Instruction& instr){
// jump directly to instruction in memory
    auto mem_addr = instr.get_mem_addr();
    this->cpu->set_pc(mem_addr);
}

void CHIP8::CALL(const Instruction& instr){
// jump to a function, saving current pc to return to
    this->cpu->push_stack(this->cpu->get_pc());
    this->cpu->set_pc(instr.get_mem_addr());
}

void CHIP8::SE_DIRECT(const Instruction& instr){
// If register value is equal to some value, skip next instruction
    if(this->cpu->get_Vx(instr.get_lhn()) == instr.get_lower_byte()){
        this->cpu->increment_pc();
    }
}

void CHIP8::SNE_DIRECT(const Instruction& instr){
// If register value is not equal to some value, skip next instruction
    if(this->cpu->get_Vx(instr.get_lhn()) != instr.get_lower_byte()){
        this->cpu->increment_pc();
    }
}

void CHIP8::SE_REG(const Instruction& instr){
// If two register values are equal to each other, skip next instruction
    if(this->cpu->get_Vx(instr.get_hln()) == this->cpu->get_Vx(instr.get_lhn())){
        this->cpu->increment_pc();
    }
}

void CHIP8::LD_DIRECT(const Instruction& instr){
// Load some value from memory into a register
    this->cpu->set_Vx(instr.get_lhn(),instr.get_lower_byte());
}

void CHIP8::ADD_DIRECT(const Instruction& instr){
// add value to register
    auto reg= instr.get_lhn();
    auto result = this->cpu->get_Vx(reg)+instr.get_lower_byte();
    this->cpu->set_Vx(reg,result);
}

void CHIP8::LD_REG(const Instruction& instr){
// set value of one register to another
    this->cpu->set_Vx(instr.get_lhn(),this->cpu->get_Vx(instr.get_hln()));
}

void CHIP8::OR(const Instruction& instr){
// OR the value of two registers, and store in the first
    auto result = this->cpu->get_Vx(instr.get_lhn()) | this->cpu->get_Vx(instr.get_hln());
    this->cpu->set_Vx(instr.get_lhn(),result);
}

void CHIP8::AND(const Instruction& instr){
// AND the value of two registers, and store in the first
    auto result = this->cpu->get_Vx(instr.get_lhn()) & this->cpu->get_Vx(instr.get_hln());
    this->cpu->set_Vx(instr.get_lhn(),result);
}

void CHIP8::XOR(const Instruction& instr){
// XOR the value of two registers, and store in the first
    auto result = this->cpu->get_Vx(instr.get_lhn()) ^ this->cpu->get_Vx(instr.get_hln());
    this->cpu->set_Vx(instr.get_lhn(),result);
}

void CHIP8::ADD(const Instruction& instr){
// add the value of two registers, and store in the first
    uint16_t result = static_cast<uint16_t>(this->cpu->get_Vx(instr.get_lhn())) + static_cast<uint16_t>(this->cpu->get_Vx(instr.get_hln()));
    this->cpu->set_Vx(instr.get_lhn(),result & 0x00FF);
// check for overflow, and set VF accordingly
    if(result & 0xFF00){
        this->cpu->set_VF(1);
    }
    else{
        this->cpu->set_VF(0);
    }
}

void CHIP8::SUB(const Instruction& instr){
// subtract the value of two registers, and store in the first
    auto vx = instr.get_lhn();
    auto vy = instr.get_hln();
    auto vx_val = this->cpu->get_Vx(vx);
    auto vy_val = this->cpu->get_Vx(vy);
    this->cpu->set_Vx(vx, vx_val - vy_val);
// set flags if underflow occurs
    if(vx_val > this->cpu->get_Vx(vx)){
        this->cpu->set_VF(1);
    }
    else{
        this->cpu->set_VF(0);
    }
}

void CHIP8::SHR(const Instruction& instr){
// shift right
    auto vx  =instr.get_lhn();
    auto vx_val = this->cpu->get_Vx(vx);
    this->cpu->set_Vx(vx,vx_val >>1);
// if LSB is 1, set flag
    if(vx_val & 0x01){
        this->cpu->set_VF(1);
    }
    else{
        this->cpu->set_VF(0);
    }
}

void CHIP8::SUBN(const Instruction& instr){
// subtract the first from the second, and store in the first
    auto vx = instr.get_lhn();
    auto vy = instr.get_hln();
    auto vx_val = this->cpu->get_Vx(vx);
    auto vy_val = this->cpu->get_Vx(vy);
    this->cpu->set_Vx(vx, vy_val - vx_val);
// set flag for overflow
    if(vy_val > this->cpu->get_Vx(vx)){
        this->cpu->set_VF(1);
    }
    else{
        this->cpu->set_VF(0);
    }
}

void CHIP8::SHL(const Instruction& instr){
// shift left, and store in register
    auto vx  =instr.get_lhn();
    auto vx_val = this->cpu->get_Vx(vx);
    this->cpu->set_Vx(vx,vx_val <<1);
// if most significant bit is 1, set flag
    if(vx_val & 0b1000'0000){
        this->cpu->set_VF(1);
    }
    else{
        this->cpu->set_VF(0);
    }
}

void CHIP8::SNE(const Instruction& instr){
// if two register values aren't equal, increment pc
    auto vx_val = this->cpu->get_Vx(instr.get_lhn());
    auto vy_val = this->cpu->get_Vx(instr.get_hln());
    if(vx_val != vy_val){
        this->cpu->increment_pc();
    }
}

void CHIP8::LD_DIRECT_I(const Instruction& instr){
// set I register to value
    this->cpu->set_I(instr.get_machine_code());
}

void CHIP8::JP_OFFSET(const Instruction& instr){
// JP to address in register 0 with some offset
    this->cpu->set_pc(this->cpu->get_Vx(0)+instr.get_mem_addr());
}

void CHIP8::RND(const Instruction& instr){
// store random number in register
    auto random_val = random_gen.roll();
    this->cpu->set_Vx(instr.get_lhn(),instr.get_lower_byte()& random_val);
}

void CHIP8::DRW(const Instruction& instr){
// draw to screen by XORing updated screen with old screen
    update_draw=true;
    auto starting_addr = this->cpu->get_I();
    int8_t starting_x_pos = (this->cpu->get_Vx(instr.get_lhn()));
    int8_t starting_y_pos = this->cpu->get_Vx(instr.get_hln());
    int8_t current_x_pos = starting_x_pos;
    int8_t current_y_pos = starting_y_pos;
    int8_t n_bytes = instr.get_lln();
    if((starting_addr+n_bytes) >= MAX_RAM_SIZE){
        throw std::invalid_argument("Trying to read sprite that's out of program memory");
    }
    bool overwritten = false;
    for(auto addr=starting_addr; addr<starting_addr+n_bytes; ++addr){
        auto cur_byte = this->mem->read(addr);
        while(cur_byte != 0){
            if(cur_byte & 0b1000'0000){ // If the top bit is set, then write to screen
                if(this->disp->write(current_x_pos, current_y_pos)){
                    overwritten = true; // check if you overwrote a pixel. If so, take note
                }
            }
            cur_byte = cur_byte << 1; // left shift to get next potential pixel
            current_x_pos += 1; // shift to the right for the next byte
            if(current_x_pos >= dis_width){
                current_x_pos -= dis_width;
            }
        }
        current_x_pos = starting_x_pos;
        current_y_pos += 1; // move onto next scanline
        if(current_y_pos >= dis_height){
            current_y_pos -= dis_height;
        }
    }
    // if a pixel is overwritten, set flag
    if(overwritten){
        this->cpu->set_VF(1);
    }
    else{
        this->cpu->set_VF(0);
    }
}

void CHIP8::SKP(const Instruction& instr){
    // skip instruction if key is pressed
    bool debug = true;
    auto expected_key = this->cpu->get_Vx(instr.get_lhn());
    auto parsed_key = this->keys->get_state();
    if(parsed_key ==  (1<< expected_key) ){
        this->cpu->increment_pc();
    }
}

void CHIP8::SKNP(const Instruction& instr){
    // skip instruction if key is not pressed
    bool debug = true;
    auto expected_key = this->cpu->get_Vx(instr.get_lhn());
    auto parsed_key = this->keys->get_state();
    if(parsed_key !=  (1<< expected_key) ){
        this->cpu->increment_pc();
    }
}

void CHIP8::LD_DELAY(const Instruction& instr){
    // load value in delay into register
    this->cpu->set_Vx(instr.get_lhn(),this->cpu->get_delay()) ;
}

void CHIP8::LD_KEY(const Instruction& instr){
    // block execution until key is pressed
    bool debug = true;
    auto reg = instr.get_lhn();
    while(this->running){
        this->update_window();
        this->tick_clock();
        auto parsed_key = this->keys->get_state();
        if(parsed_key != 0){
            auto val = 0;
            if(parsed_key & ZERO_PRESENT){ val = ZERO_KEY;}
            else if(parsed_key & ONE_PRESENT){val = ONE_KEY;}
            else if(parsed_key & TWO_PRESENT){val = TWO_KEY;}
            else if(parsed_key & THREE_PRESENT){val = THREE_KEY;}
            else if(parsed_key & FOUR_PRESENT){val = FOUR_KEY;}
            else if(parsed_key & FIVE_PRESENT){val = FIVE_KEY;}
            else if(parsed_key & SIX_PRESENT){val = SIX_KEY;}
            else if(parsed_key & SEVEN_PRESENT){val = SEVEN_KEY;}
            else if(parsed_key & EIGHT_PRESENT){val = EIGHT_KEY;}
            else if(parsed_key & NINE_PRESENT){val = NINE_KEY;}
            else if(parsed_key & A_PRESENT){val = A_KEY;}
            else if(parsed_key & B_PRESENT){val = B_KEY;}
            else if(parsed_key & C_PRESENT){val = C_KEY;}
            else if(parsed_key & D_PRESENT){val = D_KEY;}
            else if(parsed_key & E_PRESENT){val = E_KEY;}
            else if(parsed_key & F_PRESENT){val = F_KEY;}
            else{
                throw std::invalid_argument("Something went horrible wrong with key parsing...");
            }
            this->cpu->set_Vx(reg,val);
            while(1){
                this->update_window();
                this->tick_clock();
                parsed_key = this->keys->get_state();
                if(parsed_key==0){
                    break;
                }   
            }
            break;
        }
    }
}

void CHIP8::SET_DELAY(const Instruction& instr){
    // set delay timer with value in register
    this->cpu->set_delay(this->cpu->get_Vx(instr.get_lhn()));
}

void CHIP8::SET_SOUND(const Instruction& instr){
    // set sound timer with value in register
    this->cpu->set_sound(this->cpu->get_Vx(instr.get_lhn()));
}

void CHIP8::ADD_I(const Instruction& instr){
    // Add value in register to I
    auto I = this->cpu->get_I();
    auto VX = this->cpu->get_Vx(instr.get_lhn());
    this->cpu->set_I(I+VX);
}

void CHIP8::LD_SPRITE(const Instruction& instr){
// load first value of sprite data into I register
    auto val = instr.get_lhn();
    this->cpu->set_I(CHAR_OFFSET+val*5);
}

void CHIP8::STORE_BCD(const Instruction& instr){
    // store hundreds, tens, and ones of value in register into I, I+1, and I+2
    uint16_t cur_I = this->cpu->get_I();
    if(cur_I+2 > MAX_RAM_SIZE){
        throw std::invalid_argument("Can't Store BCD");
    }
    auto reg = instr.get_lhn();
    if(reg==0xF){
        throw std::invalid_argument("Can't Store Vf in BCD");
    }
    uint8_t val = this->cpu->get_Vx(reg);
    uint8_t B,C,D;
    B = (val / 100);
    C = (val/10) %10;
    D = val %10;
    this->mem->write(cur_I,B);
    this->mem->write(cur_I+1,C);
    this->mem->write(cur_I+2,D);
}

void CHIP8::STR_ARR(const Instruction& instr){
    // Store registers 0 trough largest_reg into memory starting from I
    uint8_t largest_reg = instr.get_lhn();
    uint16_t starting_addr = this->cpu->get_I();
    for(int i=0; i<=largest_reg; ++i){
        try{
            this->mem->write(starting_addr+i,this->cpu->get_Vx(i));
        }
        catch(std::invalid_argument& e){
            throw e;
        }
    }
}

void CHIP8::LD_ARR(const Instruction& instr){
    // load values from memory into registers
    uint8_t largest_reg = instr.get_lhn();
    uint16_t starting_addr = this->cpu->get_I();
    for(int i=0; i<=largest_reg; ++i){
        uint8_t mem_val;
        try{
            mem_val = this->mem->read(starting_addr+i);
        }
        catch(std::invalid_argument& e){
            throw e;
        }
        this->cpu->set_Vx(i, mem_val);
    }
}

std::string CHIP8::decode_keys(uint16_t encrypted_keys){
    std::string output="";
    if(encrypted_keys & ONE_PRESENT){output += "1";}
    if(encrypted_keys & TWO_PRESENT){output += "2";}
    if(encrypted_keys & THREE_PRESENT){output += "3";}
    if(encrypted_keys & C_PRESENT){output += "C";}
    if(encrypted_keys & FOUR_PRESENT){output += "4";}
    if(encrypted_keys & FIVE_PRESENT){output += "5";}
    if(encrypted_keys & SIX_PRESENT){output += "6";}
    if(encrypted_keys & D_PRESENT){output += "D";}
    if(encrypted_keys & SEVEN_PRESENT){output += "7";}
    if(encrypted_keys & EIGHT_PRESENT){output += "8";}
    if(encrypted_keys & NINE_PRESENT){output += "9";}
    if(encrypted_keys & E_PRESENT){output += "E";}
    if(encrypted_keys & A_PRESENT){output += "A";}
    if(encrypted_keys & ZERO_PRESENT){output += "0";}
    if(encrypted_keys & B_PRESENT){output += "B";}
    if(encrypted_keys & F_PRESENT){output += "F";}
    return output;
}