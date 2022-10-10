#include "gtest/gtest.h"

#include "Map.hpp"

class TestMap : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(TestMap, MapGivesLayerAmount)
{
    Map testMap = Map("test_data/map2_16x16_redone.tmj");
    EXPECT_EQ(testMap.getLayerAmount(), 2);
}

TEST_F(TestMap, GetGidOfFirstLayer)
{
    const int layer = 0;
    const int x = 9;
    const int y = 7;
    Map testMap = Map("test_data/map2_16x16_redone.tmj");
    EXPECT_EQ(testMap.getTiledGid(x, y, layer), 347);
}

TEST_F(TestMap, GetGidOfSecondLayer)
{
    const int layer = 1;
    const int x = 5;
    const int y = 12;
    Map testMap = Map("test_data/map2_16x16_redone.tmj");
    EXPECT_EQ(testMap.getTiledGid(x, y, layer), 1178);
}

TEST_F(TestMap, TryToGetGidFromOutOfBounds1)
{
    const int layer = 0;
    const int x = -1;
    const int y = 12;
    Map testMap = Map("test_data/map2_16x16_redone.tmj");
    EXPECT_EQ(testMap.getTiledGid(x, y, layer), 0);
}

TEST_F(TestMap, TryToGetGidFromOutOfBounds2)
{
    const int layer = 0;
    const int x = 3;
    const int y = 16;
    Map testMap = Map("test_data/map2_16x16_redone.tmj");
    EXPECT_EQ(testMap.getTiledGid(x, y, layer), 0);
}

TEST_F(TestMap, TryToGetGidFromOutOfBounds3)
{

    const int layer = 2;
    const int x = 3;
    const int y = 8;
    Map testMap = Map("test_data/map2_16x16_redone.tmj");
    EXPECT_EQ(testMap.getTiledGid(x, y, layer), 0);
}
