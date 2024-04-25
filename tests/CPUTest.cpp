#include <gtest/gtest.h>
#include "Instruction.h"
#include "CHIP8.h"

TEST(CPUTest,EncodeDecodeMachine){
    uint16_t starting = 0x1234;
    Instruction a(starting);
    Instruction b(0x01,0x02,0x03,0x04);
    EXPECT_EQ(a.get_hhb(), b.get_hhb());
    EXPECT_EQ(a.get_hlb(), b.get_hlb());
    EXPECT_EQ(a.get_lhb(), b.get_lhb());
    EXPECT_EQ(a.get_llb(), b.get_llb());
    auto val = a.get_machine_code();
    EXPECT_EQ(val, starting);
}

TEST(CPUTest, SYS){
    CHIP8 interpreter(false);
    auto instr = Instruction(0,0,0,5);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, CLS){
    CHIP8 interpreter(false);
    interpreter.disp->write(dis_width-1,dis_height-1); // Set last pixel to white
    EXPECT_EQ(interpreter.disp->read(dis_width-1,dis_height-1), WHITE);
    auto instr = Instruction(0,0,0xE,0);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
    EXPECT_EQ(interpreter.disp->read(dis_width-1,dis_height-1), BLACK);
}

TEST(CPUTest, CALL){
    CHIP8 interpreter(false);
    auto instr = Instruction(2,0xF,0xF,0xF);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_stack_size(), 1);
    EXPECT_EQ(interpreter.cpu->get_pc(), 0xFFF);
}

TEST(CPUTest, RET){
    CHIP8 interpreter(false);
    auto instr = Instruction(2,0xF,0xF,0xF);
    auto return_pc = interpreter.cpu->get_pc()+instruction_size; // once function returns, next instruction should be the one after CALL
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    instr = Instruction(0x0,0x0,0xE,0xE);
    msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_stack_size(), 0);
    EXPECT_EQ(interpreter.cpu->get_pc(), return_pc);
}

TEST(CPUTest, Jp_Direct){
    CHIP8 interpreter(false);
    // Write 0xFFFF to last valid memory slot
    interpreter.mem->write(0x0FFE,0xFF);
    interpreter.mem->write(0x0FFF,0xFF);
    // Jump to 0xFFE
    auto instr = Instruction(1,0xF,0xF,0xE);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    // check that current pc is correct
    EXPECT_EQ(interpreter.cpu->get_pc(),0x0FFE);
    // check that machine code at current pc is correct
    EXPECT_EQ(interpreter.mem->read_machine_code(interpreter.cpu->get_pc()),0xFFFF);
}


TEST(CPUTest, SE_Direct_SKIP){
    CHIP8 interpreter(false);
    interpreter.cpu->set_Vx(1,0xFF);
    auto instr = Instruction(0x3,0x1,0xF,0xF);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size*2);
}

TEST(CPUTest, SE_Direct_NO_SKIP){
    CHIP8 interpreter(false);
    interpreter.cpu->set_Vx(0,0xFF);
    auto instr = Instruction(0x3,0x0,0x0,0x0);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
}

TEST(CPUTest, SNE_Direct_SKIP){
    CHIP8 interpreter(false);
    interpreter.cpu->set_Vx(1,0xFF);
    auto instr = Instruction(0x4,0x1,0xF,0xF);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
}

TEST(CPUTest, SNE_Direct_NO_SKIP){
    CHIP8 interpreter(false);
    interpreter.cpu->set_Vx(1,0xFF);
    auto instr = Instruction(0x4,0x1,0x0,0x0);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size*2);
}

TEST(CPUTest, SEReg_SKIP){
    CHIP8 interpreter(false);
    interpreter.cpu->set_Vx(1,0xFF);
    interpreter.cpu->set_Vx(2,0xFF);
    auto instr = Instruction(0x5,0x1,0x2,0x0);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size*2);
}

TEST(CPUTest, SEReg_NO_SKIP){
    CHIP8 interpreter(false);
    interpreter.cpu->set_Vx(1,0xFF);
    interpreter.cpu->set_Vx(2,0xFE);
    auto instr = Instruction(0x5,0x2,0x1,0x0);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
}

TEST(CPUTest, LD_DIRECT){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    auto instr = Instruction(0x6,0x2,0xF,0xA);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(0x2),0xFA);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, ADD_Direct){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0xA0);
    auto instr = Instruction(0x7,0x2,0x0,0xF);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0xAF);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, LD_REG){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0xA0);
    interpreter.cpu->set_Vx(3,0x0B);
    auto instr = Instruction(0x8,0x2,0x3,0x0);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(3),0x0B);
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x0B);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, OR){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0x10);
    interpreter.cpu->set_Vx(3,0x02);
    auto instr = Instruction(0x8,0x2,0x3,0x1);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x12);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, AND){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0x10);
    interpreter.cpu->set_Vx(3,0x02);
    auto instr = Instruction(0x8,0x2,0x3,0x2);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x00);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, XOR){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0x11);
    interpreter.cpu->set_Vx(3,0x03);
    auto instr = Instruction(0x8,0x2,0x3,0x3);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x12);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, ADD){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0x11);
    interpreter.cpu->set_Vx(3,0x03);
    interpreter.cpu->set_VF(1);
    auto instr = Instruction(0x8,0x2,0x3,0x4);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x14);
    EXPECT_EQ(interpreter.cpu->get_VF(),0x00);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
    current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0xFF);
    interpreter.cpu->set_Vx(3,0x01);
    interpreter.cpu->set_VF(0);
    instr = Instruction(0x8,0x2,0x3,0x4);
    msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x00);
    EXPECT_EQ(interpreter.cpu->get_VF(),0x01);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, SUB){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0x05);
    interpreter.cpu->set_Vx(3,0x03);
    interpreter.cpu->set_VF(0);
    auto instr = Instruction(0x8,0x2,0x3,0x5);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x02);
    EXPECT_EQ(interpreter.cpu->get_VF(),0x01);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
    current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0x00);
    interpreter.cpu->set_Vx(3,0x01);
    interpreter.cpu->set_VF(1);
    instr = Instruction(0x8,0x2,0x3,0x5);
    msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0xFF);
    EXPECT_EQ(interpreter.cpu->get_VF(),0x00);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest,SHR){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0x04);
    interpreter.cpu->set_VF(1);
    auto instr = Instruction(0x8,0x2,0x3,0x6);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x02);
    EXPECT_EQ(interpreter.cpu->get_VF(),0x00);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
    current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0x03);
    interpreter.cpu->set_VF(0);
    instr = Instruction(0x8,0x2,0x3,0x6);
    msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x01);
    EXPECT_EQ(interpreter.cpu->get_VF(),0x01);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, SUBN){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0x05);
    interpreter.cpu->set_Vx(3,0x03);
    interpreter.cpu->set_VF(1);
    auto instr = Instruction(0x8,0x2,0x3,0x7);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0xFE);
    EXPECT_EQ(interpreter.cpu->get_VF(),0x00);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
    current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0x01);
    interpreter.cpu->set_Vx(3,0x02);
    interpreter.cpu->set_VF(0);
    instr = Instruction(0x8,0x2,0x3,0x7);
    msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x01);
    EXPECT_EQ(interpreter.cpu->get_VF(),0x01);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, SHL){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0x04);
    interpreter.cpu->set_VF(1);
    auto instr = Instruction(0x8,0x2,0x3,0xE);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x08);
    EXPECT_EQ(interpreter.cpu->get_VF(),0x00);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
    current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(2,0xFF);
    interpreter.cpu->set_VF(0);
    instr = Instruction(0x8,0x2,0x3,0xE);
    msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0xFE);
    EXPECT_EQ(interpreter.cpu->get_VF(),0x01  );
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, SNE){
    CHIP8 interpreter(false);
    interpreter.cpu->set_Vx(2,0x04);
    interpreter.cpu->set_Vx(3,0x05);
    auto cur_pc = interpreter.cpu->get_pc();
    auto instr = Instruction(0x9,0x2,0x3,0x0);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(),cur_pc+instruction_size*2);
    interpreter.cpu->set_Vx(2,0x04);
    interpreter.cpu->set_Vx(3,0x04);
    cur_pc = interpreter.cpu->get_pc();
    instr = Instruction(0x9,0x2,0x3,0x0);
    msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(),cur_pc+instruction_size);
}

TEST(CPUTest, LD_Direct_I){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    auto instr = Instruction(0xA,0xF,0xF,0xF);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_I(),0x0FFF);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, JP_OFFSET){
    CHIP8 interpreter(false);
    auto instr = Instruction(0xB,0x0,0x0,0x1);
    interpreter.cpu->set_Vx(0,2);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(),0x0003);
}

TEST(CPUTest, RND){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.random_gen.seed_statically();
    interpreter.random_gen.roll();
    auto value = interpreter.random_gen.roll();
    interpreter.random_gen.seed_statically();
    interpreter.random_gen.roll();
    auto expected = value & 0xFF;
    auto instr = Instruction(0xc,0x2,0xF,0xF);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),expected);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, DRW_SINGLE_LINE){
    CHIP8 interpreter(true);
    interpreter.cpu->set_Vx(0,1);
    interpreter.cpu->set_Vx(1,0);
    interpreter.cpu->set_I(CHAR_OFFSET+1); // get second line of 0 character
    auto instr = Instruction(0xD,0,1,1);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
    EXPECT_EQ(interpreter.disp->read(1,0),WHITE);
    EXPECT_EQ(interpreter.disp->read(2,0),BLACK);
    EXPECT_EQ(interpreter.disp->read(3,0),BLACK);
    EXPECT_EQ(interpreter.disp->read(4,0),WHITE);
//    SDL_Delay(1000);
}

TEST(CPUTest, DRW_SINGLE_LINE_WRAP){
    CHIP8 interpreter(true);
    interpreter.cpu->set_Vx(0,63);
    interpreter.cpu->set_Vx(1,0);
    interpreter.cpu->set_I(CHAR_OFFSET); // get second line of 0 character
    auto instr = Instruction(0xD,0,1,1);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
    EXPECT_EQ(interpreter.disp->read(63,0),WHITE);
    EXPECT_EQ(interpreter.disp->read(0,0),WHITE);
    EXPECT_EQ(interpreter.disp->read(1,0),WHITE);
    EXPECT_EQ(interpreter.disp->read(2,0),WHITE);
//    SDL_Delay(1000);
}

TEST(CPUTest, DRW_Char){
    CHIP8 interpreter(true);
    interpreter.cpu->set_Vx(0,0);
    interpreter.cpu->set_Vx(1,0);
    interpreter.cpu->set_I(CHAR_OFFSET); // get 0 character
    auto instr = Instruction(0xD,0,1,5);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
//    SDL_Delay(1000);
}

TEST(CPUTest, SKP_Key){
// This won't work, since I haven't figured out how to queue keys into Keyboard state
    CHIP8 interpreter(false);
    auto instr = Instruction(0xE,0xF,9,0xE);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(current_pc+instruction_size*2, interpreter.cpu->get_pc());
}

TEST(CPUTest,SKNP_Key){
// This will work, but for the wrong reasons
    CHIP8 interpreter(false);
    auto instr = Instruction(0xE,0xE,0xA,1);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(current_pc+instruction_size*2, interpreter.cpu->get_pc());
}

TEST(CPUTest, LD_Delay){
    CHIP8 interpreter(false);
    auto instr = Instruction(0xF,1,0,7);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(1,0);
    interpreter.cpu->set_delay(0xFF);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_NE(interpreter.cpu->get_Vx(1) ,0);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest,LD_Key_Block){
// This won't work, since I haven't figured out how to queue keys into Keyboard state
    CHIP8 interpreter(false);
    auto instr = Instruction(0xF,0,0,0xA);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
    EXPECT_EQ(interpreter.cpu->get_Vx(0),F_KEY);
}

TEST(CPUTest, SET_DELAY){
    CHIP8 interpreter(false);
    interpreter.cpu->set_Vx(1,0xFF);
    auto instr = Instruction(0xF,1,1,5);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_GE(interpreter.cpu->get_delay() ,0);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, SET_SOUND){
    CHIP8 interpreter(false);
    interpreter.cpu->set_Vx(1,0xFF);
    auto instr = Instruction(0xF,1,1,8);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_GE(interpreter.cpu->get_sound() ,0);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, ADD_I){
    CHIP8 interpreter(false);
    interpreter.cpu->set_I(0x0A00);
    interpreter.cpu->set_Vx(1,0x05);
    auto instr = Instruction(0xF,1,1,0xE);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
    EXPECT_EQ(interpreter.cpu->get_I() , 0xA05);
}

TEST(CPUTest, LoadSprite){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
// 0
    auto instr = Instruction(0xF,0x0,0x2,0x9);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*0);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
// 1
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,0x1,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*1);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
// 2
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,0x2,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*2);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 3
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,0x3,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*3);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 4
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,0x4,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*4);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 5
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,0x5,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*5);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 6
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,0x6,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*6);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 7
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,0x7,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*7);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 8
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,0x8,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*8);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 9
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,0x9,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*9);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 10
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,10,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*10);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 11
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,11,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*11);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 12
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,12,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*12);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 13
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,13,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*13);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 14
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,14,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*14);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
// 15
    current_pc = interpreter.cpu->get_pc();
    instr = Instruction(0xF,15,0x2,0x9);
    msg = interpreter.test_instruction(instr);
    EXPECT_EQ(interpreter.cpu->get_I(), CHAR_OFFSET+5*15);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    std::cout << msg << std::endl;
}

TEST(CPUTest, Store_BCD_OOB){
    CHIP8 interpreter(false);
    interpreter.cpu->set_Vx(1,0xFE);
    uint16_t starting_addr = 0x0FFF;
    interpreter.cpu->set_I(starting_addr);
    auto instr = Instruction(0xf,0x1,0x3,0x3);
    bool thrown = false;
    try{
        auto msg = interpreter.test_instruction(instr);
    }
    catch(const std::exception& e){
        thrown = true;
    }
    EXPECT_EQ(thrown, true);
}

TEST(CPUTest, Store_BCD_VF){
    CHIP8 interpreter(false);
    interpreter.cpu->set_Vx(1,0xFE);
    uint16_t starting_addr = 0x0FFF;
    interpreter.cpu->set_I(starting_addr);
    auto instr = Instruction(0xf,0xF,0x3,0x3);
    bool thrown = false;
    try{
        auto msg = interpreter.test_instruction(instr);
    }
    catch(const std::exception& e){
        thrown = true;
    }
    EXPECT_EQ(thrown, true);
}

TEST(CPUTest, Store_BCD_All_Good){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    interpreter.cpu->set_Vx(1,0xFE);
    uint16_t starting_addr = 0x0A00;
    interpreter.cpu->set_I(starting_addr);
    auto instr = Instruction(0xf,0x1,0x3,0x3);
    bool thrown = false;
    try{
        auto msg = interpreter.test_instruction(instr);
    }
    catch(const std::exception& e){
        thrown = true;
    }
    EXPECT_EQ(thrown, false);
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
    EXPECT_EQ(interpreter.mem->read(starting_addr), 2);
    EXPECT_EQ(interpreter.mem->read(starting_addr+1), 5);
    EXPECT_EQ(interpreter.mem->read(starting_addr+2), 4);
}

TEST(CPUTest, Load_Regs_All_VF){
    CHIP8 interpreter(false);
    interpreter.cpu->set_I(0xA00);
    auto instr = Instruction(0xF,0xF,0x6,0x5);
    std::string msg;
    bool thrown = false;
    try{
        msg = interpreter.test_instruction(instr);
    }
    catch(const std::exception& e){
        thrown = true;
        EXPECT_STREQ("Can't Load VF to Memory",e.what());
    }
    EXPECT_EQ(thrown,true);
}

TEST(CPUTest, Read_Regs_OOB){
    CHIP8 interpreter(false);
    interpreter.cpu->set_I(0x0FFA);
    auto instr = Instruction(0xF,0xE,0x6,0x5);
    std::string msg;
    bool thrown = false;
    try{
        msg = interpreter.test_instruction(instr);
    }
    catch(const std::exception& e){
        thrown = true;
        EXPECT_STREQ("Address out of bounds. Can't read",e.what());
    }
    EXPECT_EQ(thrown,true);
}

TEST(CPUTest, Read_Regs_Good){
// First, load values into memory
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    uint16_t starting_addr = 0x0A00;
    interpreter.cpu->set_I(starting_addr);
    for(int i=0; i<15; i++){
        interpreter.cpu->set_Vx(i,i);
    }
    auto instr = Instruction(0xF,0xE,0x5,0x5);
    bool thrown = false;
    std::string msg="";
    try{
        msg = interpreter.test_instruction(instr);
    }
    catch(const std::exception& e){
        thrown = true;
    }
    std::cout << msg << std::endl;
    EXPECT_EQ(thrown,false);
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
    for(int i=0; i<15; i++){
        EXPECT_EQ(interpreter.mem->read(0xA00+i),i);
    }

    instr = Instruction(0xF,0xE,0x6,0x5);
    current_pc = interpreter.cpu->get_pc();
    thrown = false;
    msg="";
    try{
        msg = interpreter.test_instruction(instr);
    }
    catch(const std::exception& e){
        thrown = true;
    }
    std::cout << msg << std::endl;
    EXPECT_EQ(thrown,false);
    for(int i=0; i<15; i++){
        EXPECT_EQ(interpreter.cpu->get_Vx(i),i);
    }
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, Store_Regs_All_VF){
    CHIP8 interpreter(false);
    interpreter.cpu->set_I(0xA00);
    auto instr = Instruction(0xF,0xF,0x5,0x5);
    std::string msg;
    bool thrown = false;
    try{
        msg = interpreter.test_instruction(instr);
    }
    catch(const std::exception& e){
        thrown = true;
        EXPECT_STREQ("Can't Save VF to Memory",e.what());
    }
    EXPECT_EQ(thrown,true);
}

TEST(CPUTest, Store_Regs_All_Ok){
    CHIP8 interpreter(false);
    auto current_pc = interpreter.cpu->get_pc();
    uint16_t starting_addr = 0x0A00 ;
    interpreter.cpu->set_I(starting_addr);
    for(int i=0; i<15; i++){
        interpreter.cpu->set_Vx(i,i);
    }
    auto instr = Instruction(0xF,0xE,0x5,0x5);
    bool thrown = false;
    std::string msg="";
    try{
        msg = interpreter.test_instruction(instr);
    }
    catch(const std::exception& e){
        thrown = true;
    }
    std::cout << msg << std::endl;
    EXPECT_EQ(thrown,false);
    for(int i=0; i<15; i++){
        EXPECT_EQ(interpreter.mem->read(0xA00+i),i);
    }
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, Store_Regs_OOB){
    CHIP8 interpreter(false);
    interpreter.cpu->set_I(0x0FFA);
    auto instr = Instruction(0xF,0xE,0x5,0x5);
    std::string msg;
    bool thrown = false;
    try{
        msg = interpreter.test_instruction(instr);
    }
    catch(const std::exception& e){
        thrown = true;
        EXPECT_STREQ("Address out of bounds. Can't write",e.what());
    }
    EXPECT_EQ(thrown,true);
}