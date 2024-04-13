#include <gtest/gtest.h>
#include <chrono>
#include <thread>

#include "CPU.h"

TEST(TimerTest, Delay){
    CPU current_cpu;
    EXPECT_EQ(current_cpu.get_delay(),0);
    current_cpu.set_delay(60);
    EXPECT_GE(current_cpu.get_delay(),0);
    using namespace std::this_thread;
    using namespace std::chrono_literals;
    using std::chrono::system_clock;
    sleep_for(1s);
    sleep_until(system_clock::now() + 1s);
    EXPECT_EQ(current_cpu.get_delay(),0);
}