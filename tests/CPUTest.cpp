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
    CHIP8 interpreter(false,false);
    auto instr = Instruction(0,0,0,5);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(current_pc+instruction_size, interpreter.cpu->get_pc());
}

TEST(CPUTest, CLS){
    CHIP8 interpreter(false,false);
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
    CHIP8 interpreter(false,false);
    auto instr = Instruction(2,0xF,0xF,0xF);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_stack_size(), 1);
    EXPECT_EQ(interpreter.cpu->get_pc(), 0xFFF);
}

TEST(CPUTest, RET){
    CHIP8 interpreter(false,false);
    auto instr = Instruction(2,0xF,0xF,0xF);
    auto old_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    instr = Instruction(0x0,0x0,0xE,0xE);
    msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_stack_size(), 0);
    EXPECT_EQ(interpreter.cpu->get_pc(), old_pc);
}

TEST(CPUTest, Jp_Direct){
    CHIP8 interpreter(false,false);
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
    CHIP8 interpreter(false,false);
    interpreter.cpu->set_Vx(1,0xFF);
    auto instr = Instruction(0x3,0x1,0xF,0xF);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size*2);
}

TEST(CPUTest, SE_Direct_NO_SKIP){
    CHIP8 interpreter(false,false);
    interpreter.cpu->set_Vx(0,0xFF);
    auto instr = Instruction(0x3,0x0,0x0,0x0);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
}

TEST(CPUTest, SNE_Direct_SKIP){
    CHIP8 interpreter(false,false);
    interpreter.cpu->set_Vx(1,0xFF);
    auto instr = Instruction(0x4,0x1,0xF,0xF);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
}

TEST(CPUTest, SNE_Direct_NO_SKIP){
    CHIP8 interpreter(false,false);
    interpreter.cpu->set_Vx(1,0xFF);
    auto instr = Instruction(0x4,0x1,0x0,0x0);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size*2);
}

TEST(CPUTest, SEReg_SKIP){
    CHIP8 interpreter(false,false);
    interpreter.cpu->set_Vx(1,0xFF);
    interpreter.cpu->set_Vx(2,0xFF);
    auto instr = Instruction(0x5,0x1,0x2,0x0);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size*2);
}

TEST(CPUTest, SEReg_NO_SKIP){
    CHIP8 interpreter(false,false);
    interpreter.cpu->set_Vx(1,0xFF);
    interpreter.cpu->set_Vx(2,0xFE);
    auto instr = Instruction(0x5,0x2,0x1,0x0);
    auto current_pc = interpreter.cpu->get_pc();
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_pc(), current_pc+instruction_size);
}

TEST(CPUTest, LD_DIRECT){
    CHIP8 interpreter(false,false);
    auto instr = Instruction(0x6,0x2,0xF,0xA);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(0x2),0xFA);
}

TEST(CPUTest, ADD_Direct){
    CHIP8 interpreter(false,false);
    interpreter.cpu->set_Vx(2,0xA0);
    auto instr = Instruction(0x7,0x2,0x0,0xF);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0xAF);
}

TEST(CPUTest, LD_REG){
    CHIP8 interpreter(false,false);
    interpreter.cpu->set_Vx(2,0xA0);
    interpreter.cpu->set_Vx(3,0x0B);
    auto instr = Instruction(0x8,0x2,0x3,0x0);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(3),0x0B);
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x0B);
}

TEST(CPUTest, OR){
    CHIP8 interpreter(false,false);
    interpreter.cpu->set_Vx(2,0x10);
    interpreter.cpu->set_Vx(3,0x02);
    auto instr = Instruction(0x8,0x2,0x3,0x1);
    auto msg = interpreter.test_instruction(instr);
    std::cout << msg << std::endl;
    EXPECT_EQ(interpreter.cpu->get_Vx(2),0x12);
}