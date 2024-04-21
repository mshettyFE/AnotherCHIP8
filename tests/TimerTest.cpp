#include <gtest/gtest.h>
#include <chrono>
#include <thread>

#include <SDL2/SDL.h>
#include "CHIP8.h"
#include <iostream>
#include "CPU.h"

TEST(TimerTest, Delay){
    CPU current_cpu;
    EXPECT_EQ(current_cpu.get_delay(),0);
    current_cpu.set_delay(60);
    EXPECT_GE(current_cpu.get_delay(),0);
    SDL_Delay(1100);
    EXPECT_EQ(current_cpu.get_delay(),0);
}

TEST(TimerTest, Sound){
    CPU current_cpu;
    EXPECT_EQ(current_cpu.get_sound(),0);
    current_cpu.set_sound(60);
    EXPECT_GE(current_cpu.get_sound(),0);
    SDL_Delay(1100);
    EXPECT_EQ(current_cpu.get_sound(),0);
    SDL_Quit();
}