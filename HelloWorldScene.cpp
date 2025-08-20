/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "models/CardModel.h"
#include "views/CardSprite.h"
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Create the playfield layer (shit yellow)
    auto playfieldLayer = LayerColor::create(Color4B(204, 153, 51, 255), 1080, 1500);
    playfieldLayer->setPosition(Vec2(0, 580));
    this->addChild(playfieldLayer, 0);

    // Create the stack layer (purple)
    auto stackLayer = LayerColor::create(Color4B(128, 0, 128, 255), 1080, 580);
    stackLayer->setPosition(Vec2(0, 0));
    this->addChild(stackLayer, 0);

    std::string fullPath = FileUtils::getInstance()->fullPathForFilename("res/level_0.json");
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(fullPath);

    rapidjson::Document doc;
    doc.Parse(contentStr.c_str());

    if (doc.HasMember("Playfield"))
    {
        const rapidjson::Value& playfield = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfield.Size(); i++)
        {
            const rapidjson::Value& cardInfo = playfield[i];
            auto card = CardSprite::create(
                static_cast<CardFaceType>(cardInfo["CardFace"].GetInt()),
                static_cast<CardSuitType>(cardInfo["CardSuit"].GetInt())
            );
            card->setPosition(Vec2(cardInfo["Position"]["x"].GetFloat(), cardInfo["Position"]["y"].GetFloat()));
            playfieldLayer->addChild(card);
        }
    }

    if (doc.HasMember("Stack"))
    {
        const rapidjson::Value& stack = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < stack.Size(); i++)
        {
            const rapidjson::Value& cardInfo = stack[i];
            auto card = CardSprite::create(
                static_cast<CardFaceType>(cardInfo["CardFace"].GetInt()),
                static_cast<CardSuitType>(cardInfo["CardSuit"].GetInt())
            );
            // Stagger the cards in the stack for visibility
            card->setPosition(Vec2(visibleSize.width / 2, 200 + i * 5));
            stackLayer->addChild(card);
        }
    }

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
