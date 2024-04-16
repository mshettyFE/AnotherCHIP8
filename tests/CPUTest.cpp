#include <gtest/gtest.h>
#include "Instruction.h"
#include "CPU.h"


TEST(CPUTest,EncodeDecode){
    uint16_t starting = 0x1234;
    Instruction a(starting);
    Instruction b(0x01,0x02,0x03,0x04);
    EXPECT_EQ(a.get_hhb(), b.get_hhb());
    EXPECT_EQ(a.get_hlb(), b.get_hlb());
    EXPECT_EQ(a.get_lhb(), b.get_lhb());
    EXPECT_EQ(a.get_llb(), b.get_llb());
    auto val = a.emit();
    EXPECT_EQ(val, starting);
}