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

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "models/CardModel.h"
#include "views/CardSprite.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <vector>

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp");
}

bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    initLayers();
    loadAndProcessCards();

    return true;
}

void GameScene::initLayers()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    playfieldLayer = LayerColor::create(Color4B(204, 153, 51, 255), 1080, 1500);
    playfieldLayer->setPosition(Vec2(0, 580));
    this->addChild(playfieldLayer, 0);

    stackLayer = LayerColor::create(Color4B(128, 0, 128, 255), 1080, 580);
    stackLayer->setPosition(Vec2(0, 0));
    this->addChild(stackLayer, 0);
}

void GameScene::loadAndProcessCards()
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename("res/level_0.json");
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(fullPath);

    rapidjson::Document doc;
    doc.Parse(contentStr.c_str());

    if (doc.HasMember("Playfield"))
    {
        setupPlayfieldCards(doc["Playfield"]);
    }

    if (doc.HasMember("Stack"))
    {
        setupStackCards(doc["Stack"]);
    }
}

void GameScene::setupPlayfieldCards(const rapidjson::Value& playfieldData)
{
    for (rapidjson::SizeType i = 0; i < playfieldData.Size(); i++)
    {
        const rapidjson::Value& cardInfo = playfieldData[i];
        auto card = CardSprite::create(
            static_cast<CardFaceType>(cardInfo["CardFace"].GetInt()),
            static_cast<CardSuitType>(cardInfo["CardSuit"].GetInt())
        );
        card->setPosition(Vec2(cardInfo["Position"]["x"].GetFloat(), cardInfo["Position"]["y"].GetFloat()));
        playfieldLayer->addChild(card);
    }
}

void GameScene::setupStackCards(const rapidjson::Value& stackData)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    int numCards = stackData.Size();
    if (numCards <= 0) return;

    std::vector<CardSprite*> partACards;
    CardSprite* partBCard = nullptr;

    for (rapidjson::SizeType i = 0; i < stackData.Size(); i++)
    {
        const rapidjson::Value& cardInfo = stackData[i];
        auto card = CardSprite::create(
            static_cast<CardFaceType>(cardInfo["CardFace"].GetInt()),
            static_cast<CardSuitType>(cardInfo["CardSuit"].GetInt())
        );
        
        if (i == numCards - 1) {
            partBCard = card;
        } else {
            partACards.push_back(card);
        }
        stackLayer->addChild(card);
    }

    float cardWidth = partACards.empty() ? partBCard->getContentSize().width : partACards[0]->getContentSize().width;
    float yPosition = 300.0f;
    float gap = 40.0f;

    // Layout for Part A
    float totalWidthA = 0;
    float spacingA = 0;
    int numPartACards = partACards.size();
    if (numPartACards > 0) {
        spacingA = cardWidth * 0.8f;
        totalWidthA = cardWidth + (numPartACards - 1) * spacingA;
    }

    // Layout for Part B
    float totalWidthB = (partBCard != nullptr) ? cardWidth : 0;
    
    // Total layout and starting position
    float totalLayoutWidth = totalWidthA + totalWidthB + (numPartACards > 0 && partBCard != nullptr ? gap : 0);
    float startX = (visibleSize.width - totalLayoutWidth) / 2.0f;

    // Position Part A cards
    float currentX = startX;
    for (int i = 0; i < numPartACards; ++i)
    {
        partACards[i]->setPosition(Vec2(currentX + cardWidth / 2.0f, yPosition));
        currentX += spacingA;
    }

    // Position Part B card
    if (partBCard != nullptr) {
        currentX = startX + totalWidthA + (numPartACards > 0 ? gap : 0);
        partBCard->setPosition(Vec2(currentX + cardWidth / 2.0f, yPosition));
    }
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

