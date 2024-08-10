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

TEST_F(TestJson, ReadMapTileHeight)
{
    cJSON* json;
    cJSON* tileheight;

    const std::string file_path = "test_data/map2_16x16_redone.tmj";

    json = 0;
    tileheight = 0;

    json = Json::readFromFile(file_path);

    tileheight = cJSON_GetObjectItemCaseSensitive(json, "tileheight");

    EXPECT_EQ(32, tileheight->valueint); // Test is yet to be written
}