#include "views/GameScene.h"
#include "controllers/GameController.h"
#include "models/CardModel.h"
#include "views/CardSprite.h"
#include "SimpleAudioEngine.h"
#include <vector>

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    _controller = new GameController(this);
    _controller->init();

    _initLayers();
    _setupUI();

    return true;
}

void GameScene::_initLayers()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    _playfieldLayer = LayerColor::create(Color4B(204, 153, 51, 255), 1080, 1500);
    _playfieldLayer->setPosition(Vec2(0, 580));
    this->addChild(_playfieldLayer, 0);

    _stackLayer = LayerColor::create(Color4B(128, 0, 128, 255), 1080, 580);
    _stackLayer->setPosition(Vec2(0, 0));
    this->addChild(_stackLayer, 0);
}

void GameScene::_setupUI()
{
    // Setup playfield cards
    const auto& playfieldCards = _controller->getPlayfieldCards();
    for (const auto* cardModel : playfieldCards)
    {
        auto card = CardSprite::create(cardModel->face, cardModel->suit);
        card->setPosition(cardModel->position);
        _playfieldLayer->addChild(card);
    }

    // Setup stack cards
    const auto& stackCardModels = _controller->getStackCards();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    int numCards = stackCardModels.size();
    if (numCards <= 0) return;

    std::vector<CardSprite*> partACards;
    CardSprite* partBCard = nullptr;

    for (int i = 0; i < numCards; ++i)
    {
        const auto* cardModel = stackCardModels[i];
        auto card = CardSprite::create(cardModel->face, cardModel->suit);
        
        if (i == numCards - 1) {
            partBCard = card;
        } else {
            partACards.push_back(card);
        }
        _stackLayer->addChild(card);
    }

    float cardWidth = 0;
    if (partBCard) {
        cardWidth = partBCard->getContentSize().width;
    } else if (!partACards.empty()) {
        cardWidth = partACards[0]->getContentSize().width;
    }

    if (cardWidth == 0) return; // No cards to layout

    float yPosition = _stackLayer->getContentSize().height / 2;
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

