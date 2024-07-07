#include "gtest/gtest.h"

#include "Json.hpp"

class TestJson : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(TestJson, ParseJsonFromFile)
{
    cJSON* json;

    const std::string file_path = "test_data/map2_16x16_redone.tmj";

    json = 0;

    json = Json::readFromFile(file_path);

    EXPECT_NE(json, nullptr);
}

// TODO read tileheight and match to be '32'
//
TEST_F(TestJson, X)
{
    EXPECT_EQ(1, 0); // Test is yet to be written
}