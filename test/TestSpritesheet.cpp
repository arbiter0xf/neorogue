#include "gtest/gtest.h"

#include "GraphicsUtil.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"

class TestSpritesheet : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(TestSpritesheet, SpritesheetJsonLoadsSuccessfully)
{
    boost::json::object frameObjectToTest;
    boost::json::value configToTest;
    boost::json::object configToTestObj;
    boost::json::value tPackerJsonValue;
    boost::json::object tPackerFramesObj;

    spritesheet_pool spritesheetPool;
    GraphicsUtil::loadSpritesheets(spritesheetPool);
    Spritesheet spritesheet;

    for (Spritesheet s : spritesheetPool) {
        if (0 == s.getName().compare("dc-mon")) {
            spritesheet = s;
            break;
        }
    }

    EXPECT_EQ(0, spritesheet.getName().compare("dc-mon"));

    tPackerJsonValue = spritesheet.getJson();
    tPackerFramesObj = Json::getFirstInnerObject(tPackerJsonValue);

    auto framesIter = tPackerFramesObj.cbegin();
    auto framesIterEnd = tPackerFramesObj.cend();

    while (framesIter != framesIterEnd) {
        const auto& [tileName, tileConfig] = *framesIter;

        if (0 == tileName.compare("animals/butterfly5.png")) {
            configToTest = tileConfig;
            break;
        }

        framesIter++;
    }

    EXPECT_NE(framesIter, framesIterEnd);

    configToTestObj = configToTest.as_object();
    frameObjectToTest = Json::getValueWithKey("frame", configToTestObj).as_object();

    // "animals/butterfly5.png":
    // {
    //     "frame": {"x":302,"y":545,"w":29,"h":30},
    //     "rotated": false,
    //     "trimmed": true,
    //     "spriteSourceSize": {"x":3,"y":2,"w":29,"h":30},
    //     "sourceSize": {"w":32,"h":32}
    // },
    EXPECT_EQ(Json::getValueWithKey("x", frameObjectToTest).as_int64(), 302);
    EXPECT_EQ(Json::getValueWithKey("y", frameObjectToTest).as_int64(), 545);
    EXPECT_EQ(Json::getValueWithKey("w", frameObjectToTest).as_int64(), 29);
    EXPECT_EQ(Json::getValueWithKey("h", frameObjectToTest).as_int64(), 30);
}
