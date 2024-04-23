#include <gtest/gtest.h>

#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>
#include "CPU.h"

TEST(TimerTest, Delay){
    CHIP8 interpreter(false);
    EXPECT_EQ(interpreter.cpu->get_delay(),0);
    interpreter.cpu->set_delay(60);
    EXPECT_GE(interpreter.cpu->get_delay(),0);
    SDL_Delay(1000); // wait for 1 sec
    // run any instruction to update counter
    auto instr = Instruction(0x0,0xF,0xF,0xF);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_delay(),0);
}

TEST(TimerTest, Sound){
    CHIP8 interpreter(false);
    EXPECT_EQ(interpreter.cpu->get_sound(),0);
    interpreter.cpu->set_sound(60);
    EXPECT_GE(interpreter.cpu->get_sound(),0);
     SDL_Delay(1000); // wait for 1 sec
    auto instr = Instruction(0x0,0xF,0xF,0xF);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_sound(),0);
}