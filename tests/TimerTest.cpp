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
    using namespace std::this_thread;
    using namespace std::chrono_literals;
    using std::chrono::steady_clock;
    sleep_for(1s);
    sleep_until(steady_clock::now() + 1s);
    EXPECT_EQ(current_cpu.get_delay(),0);
}

TEST(TimerTest, Sound){
    CHIP8 interpreter(false);
    EXPECT_EQ(interpreter.cpu.get_sound(),0);
    interpreter.cpu.set_sound(60);
    interpreter.cpu.print();
    EXPECT_GE(interpreter.cpu.get_sound(),0);
    using namespace std::this_thread;
    using namespace std::chrono_literals;
    using std::chrono::steady_clock;
    sleep_for(1s);
    sleep_until(steady_clock::now() + 1s);
    EXPECT_EQ(interpreter.cpu.get_sound(),0);
    interpreter.cpu.print();
    SDL_Quit();
}