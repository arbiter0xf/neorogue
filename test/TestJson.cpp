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
    nlohmann::json json;

    const std::string file_path = "test_data/map2_16x16_redone.tmj";

    json = nullptr;
    json = Json::readFromFile(file_path);

    EXPECT_NE(json, nullptr);
}

TEST_F(TestJson, ReadMapTileHeight)
{
    nlohmann::json json;
    uint64_t tileheight;

    const std::string file_path = "test_data/map2_16x16_redone.tmj";

    tileheight = 0;
    json = nullptr;

    json = Json::readFromFile(file_path);
    tileheight = json["tileheight"];

    EXPECT_EQ(32, tileheight);
}
