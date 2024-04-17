#ifndef CHIP8_H
#define CHIP8_H

#include "CPU.h"
#include "Display.h"
#include "Memory.h"
#include "Keyboard.h"
#include <type_traits>

// Good references
// [1] https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
// [2] http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

class CHIP8{
private:
    bool loaded=true;
// assembly instructions
    int SYS(const Instruction& instr);
    int CLS(const Instruction& instr);
    int RET(const Instruction& instr);
    int JP_DIRECT(const Instruction& instr);
    int CALL_DIRECT(const Instruction& instr);
    int SE_DIRECT(const Instruction& instr);
    int SNE_DIRECT(const Instruction& instr);
    int SE_REG(const Instruction& instr);
    int LD_DIRECT(const Instruction& instr);
    int ADD_DIRECT(const Instruction& instr);
    int LD_REG(const Instruction& instr);
    int OR(const Instruction& instr);
    int AND(const Instruction& instr);
    int XOR(const Instruction& instr);
    int ADD(const Instruction& instr);
    int SUB(const Instruction& instr);
    int SHR(const Instruction& instr);
    int SUBN(const Instruction& instr);
    int SHL(const Instruction& instr);
    int SNE(const Instruction& instr);
    int LD_DIRECT_I(const Instruction& instr);
    int JP_OFFSET(const Instruction& instr);
    int RND(const Instruction& instr);
    int DRW(const Instruction& instr);
    int SKP(const Instruction& instr);
    int SKPN(const Instruction& instr);
    int LD_DELAY(const Instruction& instr);
    int LD_KEY(const Instruction& instr);
    int SET_DELAY(const Instruction& instr);
    int SET_SOUND(const Instruction& instr);
    int ADD_I(const Instruction& instr);
    int LD_SPRITE(const Instruction& instr);
    int STORE_BCD(const Instruction& instr);
    int LD_ARR(const Instruction& instr);
    int LOAD_BCD(const Instruction& instr);

    template<
        typename T,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    >
    std::string hex_to_string(T variable){
        std::stringstream out;
        out << std::hex << instr.get_machine_code();
        return out.str();
    }
    std::string decoding_error(const Instruction& instr);
// function pointer to arbitrary assembly instruction
    typedef int(CHIP8::*assembly_func)(const Instruction&);
// Take a bundled instruction and map it to the appropriate assembly instruction
    assembly_func decode(const Instruction& instr, std::string& out_msg, bool debug=false);
// take an assembly instruction, and do it (debug is is to print out debug info)
    int execute(assembly_func fnc, const Instruction& instr);
public:
    CPU cpu;
    Display disp;
    Memory mem;
    KeyPad keys;
    CHIP8(bool visible=true);
    void print() const;
    void load(std::string file);
    std::string disassemble();
    uint16_t fetch() const;

    Instruction bundle(uint16_t instruction) const; // wrap machine code instruction into a class for easier handling
    std::string decompile(const Instruction& instr); // convert instruction to associated assembly instruction

    std::string test_instruction(const Instruction& instr); // access private members decode and execute for a given instruction
};

bool is_big_endian(void);
#endif