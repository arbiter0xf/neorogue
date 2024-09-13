#include <climits>

#include "gtest/gtest.h"

// Disable name mangling so that SDL can find and redefine main.
// https://djrollins.com/2016/10/02/sdl-on-windows/
extern "C" int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
