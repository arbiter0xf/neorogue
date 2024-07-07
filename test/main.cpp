#if 0
// Header-only with multiple translation units
// https://www.boost.org/doc/libs/1_69_0/libs/test/doc/html/boost_test/adv_scenarios/single_header_customizations/multiple_translation_units.html
// Must to include boost/json/src.hpp in only one source file and other files
// must then include boost/json.hpp.
#include <boost/json/src.hpp>
#endif

#include <climits>

#include "gtest/gtest.h"

// Disable name mangling so that SDL can find and redefine main.
// https://djrollins.com/2016/10/02/sdl-on-windows/
extern "C" int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
