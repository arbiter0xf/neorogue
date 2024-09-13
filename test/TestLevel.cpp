#include "gtest/gtest.h"

#include "Level.hpp"

class TestLevel : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

// TODO Find out if Level is an outdated concept and remove if so.
TEST_F(TestLevel, GetTestLevel1TileDescriptions)
{
    const int descriptionX10Y0 = 11;
    const int descriptionX0Y12 = 13;
    const int descriptionX15Y8 = 1;

    Level testLevel1 = Level("levels/test_level1.txt");

    EXPECT_EQ(descriptionX10Y0, testLevel1.getTileId(10, 0));
    EXPECT_EQ(descriptionX0Y12, testLevel1.getTileId(0, 12));
    EXPECT_EQ(descriptionX15Y8, testLevel1.getTileId(15, 8));
}
