#ifndef CHIP8_H
#define CHIP8_H

#include "CPU.h"
#include "Display.h"
#include "Memory.h"
#include "Input.h"
#include "RNG.h"
#include <iomanip>
#include <type_traits>
#include <sstream>
#include <memory>

// Good references
// [1] https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
// [2] http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

class CHIP8{
private:
    bool loaded=true;
    bool update_draw = false;
    bool running = true;
// assembly instructions
    void SYS(const Instruction& instr);
    void CLS(const Instruction& instr);
    void RET(const Instruction& instr);
    void JP_DIRECT(const Instruction& instr);
    void CALL(const Instruction& instr);
    void SE_DIRECT(const Instruction& instr);
    void SNE_DIRECT(const Instruction& instr);
    void SE_REG(const Instruction& instr);
    void LD_DIRECT(const Instruction& instr);
    void ADD_DIRECT(const Instruction& instr);
    void LD_REG(const Instruction& instr);
    void OR(const Instruction& instr);
    void AND(const Instruction& instr);
    void XOR(const Instruction& instr);
    void ADD(const Instruction& instr);
    void SUB(const Instruction& instr);
    void SHR(const Instruction& instr);
    void SUBN(const Instruction& instr);
    void SHL(const Instruction& instr);
    void SNE(const Instruction& instr);
    void LD_DIRECT_I(const Instruction& instr);
    void JP_OFFSET(const Instruction& instr);
    void RND(const Instruction& instr);
    void DRW(const Instruction& instr);
    void SKP(const Instruction& instr);
    void SKNP(const Instruction& instr);
    void LD_DELAY(const Instruction& instr);
    void LD_KEY(const Instruction& instr);
    void SET_DELAY(const Instruction& instr);
    void SET_SOUND(const Instruction& instr);
    void ADD_I(const Instruction& instr);
    void LD_SPRITE(const Instruction& instr);
    void STORE_BCD(const Instruction& instr);
    void STR_ARR(const Instruction& instr);
    void LD_ARR(const Instruction& instr);

    template<
        typename T,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    std::string hex_to_string(T variable){
        std::stringstream ss;
        ss << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << static_cast<unsigned int>(variable);
        return ss.str();
    }
    std::string decoding_error(const Instruction& instr);
// Grab binary of current instruction
    uint16_t fetch() const;
// function pointer to arbitrary assembly instruction
    typedef void(CHIP8::*assembly_func)(const Instruction&);
// Take a bundled instruction and map it to the appropriate assembly instruction
    assembly_func decode(const Instruction& instr, std::string& out_msg, bool debug=false);
// take an assembly instruction, and do it (debug is is to print out debug info)
    void execute(assembly_func fnc, const Instruction& instr);
public:

    std::unique_ptr<CPU> cpu;
    std::unique_ptr<Display> disp;
    std::unique_ptr<Memory> mem;
    std::unique_ptr<KeyPad> keys;

    RNG random_gen;

    CHIP8(bool visible=true, bool threading=true);
    ~CHIP8(){SDL_Quit();}

    void print() const;
    void load(std::string file); // assumes file is big_endian
    std::string disassemble(bool raw=false);

    void queue_key(const KEYS_MAPS& ky);

    void reset();

    Instruction bundle(uint16_t instruction) const; // wrap machine code instruction into a class for easier handling
    std::string decompile(const Instruction& instr); // convert instruction to associated assembly instruction

    std::string test_instruction(const Instruction& instr); // access private members decode and execute for a given instruction

    void run_eternal(bool verbose=false);

    void update_window();

};

#endif