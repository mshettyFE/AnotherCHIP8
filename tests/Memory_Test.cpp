#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>
#include "CPU.h"

TEST(MemTest,LoadInstruction){
    CHIP8 interpreter;
    uint8_t first = 0xF0;
    uint8_t second =  0x90;
    uint16_t test_value  = ((uint16_t) first) << 8 | second;
    auto val =  interpreter.mem.read_machine_code(0x50);
    EXPECT_EQ(test_value, val);
}

TEST(MemTest, LoadROM){
    CHIP8 interpreter;
    interpreter.load("../tests/IBM.ch8");
    interpreter.mem.dump();
    EXPECT_EQ(interpreter.mem.read(0x200),0xe0);
    EXPECT_EQ(interpreter.mem.read(0x201),0x00);
    EXPECT_EQ(interpreter.mem.read(0x276),0xe0);
    EXPECT_EQ(interpreter.mem.read(0x277),0x00);
    EXPECT_EQ(interpreter.mem.read(0x220),0x1F);
    EXPECT_EQ(interpreter.mem.read(0x221),0xD0);
}
