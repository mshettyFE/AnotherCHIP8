#ifndef CHIP8_H
#define CHIP8_H

#include "SoundCard.h"
#include "CPU.h"
#include "Display.h"
#include "Memory.h"
#include "Input.h"
#include "RNG.h"
#include "Keyboard.h"
#include <iomanip>
#include <type_traits>
#include <sstream>
#include <memory>
#include <chrono>

class CHIP8{
private:
    bool loaded=true; // wheather a ROM has been loaded or not
    bool update_draw = false; // flag to see if we need to update the screen
    bool running = true; // flag to check if we should keep reading instructions
    bool paused = false; // flag to check if during run_eternal, we should enter a pause loop
    std::chrono::steady_clock::time_point last_instruction_time; // time last instruction finished executing. Used to sync the frame rate
    std::chrono::steady_clock::time_point last_keyboard_time; // time since the keyboard queue was emptied. Used to poll client input at 60 Hz

    SDL_Event event;
// assembly instructions
// see http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
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

    // Restrict T to be a numerical object, so that hex_to_string makes sense
    template<typename T,typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    // convenience function to print number as hex string of fixed width 4
    std::string hex_to_string(T variable){
        std::stringstream ss;
        ss << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << static_cast<unsigned int>(variable);
        return ss.str();
    }

    // Error handler for a particular instruction
    std::string decoding_error(const Instruction& instr);
    
    // Grab binary of current instruction
    uint16_t fetch() const;
    
    // function pointer to arbitrary assembly instruction
    typedef void(CHIP8::*assembly_func)(const Instruction&);
    
    // Take an Instruction and map it to the appropriate assembly instruction
    assembly_func decode(const Instruction& instr, std::string& out_msg, bool debug=false);
    
    // take an assembly instruction and it's corresponding function and just do it
    void execute(assembly_func fnc, const Instruction& instr);
public:

    // hardware
    std::unique_ptr<CPU> cpu;
    std::unique_ptr<Display> disp;
    std::unique_ptr<Memory> mem;
    std::unique_ptr<Keyboard> keys;
    std::unique_ptr<SoundCard> sound;

    // global RNG
    RNG random_gen;

    // can toggle visibility on/off on construction. Useful for unit tests, since the screen constantly popping in and out is annoying
    CHIP8(bool visible=true);
    // Need Destructor to call appropriate SDL calls
    ~CHIP8(){SDL_Quit();}

    void print() const; // calls print functions of underlying hardware
    void load(std::string file); // assumes file is big_endian
    std::string disassemble(bool raw=false); // Does a hex dump of program memory, and prints out associated instructions if it can be interpreted
    // not all instructions are parsable, and that's OK. It could be data

    void set_run(bool val){this->running = val;}
    bool get_run(){return this->running;}

    void set_pause(bool val){this->paused = val;}
    bool get_pause(){return this->paused;}

    void reset(); // Hard reset system to known starting state

    std::string decompile(const Instruction& instr); // convert instruction to associated assembly instruction

    std::string test_instruction(const Instruction& instr); // access private members decode and execute for a given instruction

    void run_eternal(bool verbose=false, bool display=true); // run forever until the heat death of the universe
    void run_iterations(unsigned int count=1000, bool verbose=true, bool display=true); // run for a given number of iterations

    void update_window(bool debug =false); // Given current system state, update screen. Also handle SDL mouse events (ie. Close window)
    void tick_clock(); // deal with all the timing stuff, like advancing sound and delay registers, playing sound, updating keyboard state, maintaining frame rate
    void tick_clock_paused(); // deal with timing stuff when game is paused

    std::string decode_keys(uint16_t encrypted_keys); // given the keyboard state encoded as 16bit number, print all the keys that were pressed

    bool get_running() const{return this->running;}
};

#endif