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

TEST_F(TestLevel, GetTestLevel1TileDescriptions)
{
    const std::string descriptionX10Y0 = "11";
    const std::string descriptionX0Y12 = "13";
    const std::string descriptionX15Y8 = "1";

    Level testLevel1 = Level("levels/test_level1.txt");

    EXPECT_EQ(descriptionX10Y0, testLevel1.getTileDescription(10, 0));
    EXPECT_EQ(descriptionX0Y12, testLevel1.getTileDescription(0, 12));
    EXPECT_EQ(descriptionX15Y8, testLevel1.getTileDescription(15, 8));
}
