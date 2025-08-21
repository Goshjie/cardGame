#include "views/CardLayer.h"
#include "controllers/GameController.h"
#include <algorithm>
#include <stack>

USING_NS_CC;

CardLayer* CardLayer::create(GameController* controller)
{
    CardLayer *pRet = new(std::nothrow) CardLayer();
    if (pRet && pRet->init(controller))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool CardLayer::init(GameController* controller)
{
    if (!Layer::init())
    {
        return false;
    }

    _controller = controller;

    _initLayers();
    _setupUI();

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(CardLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(CardLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(CardLayer::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(CardLayer::onTouchCancelled, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _touchInfoLabel = Label::createWithTTF("Touch Info", "fonts/Marker Felt.ttf", 48);
    _touchInfoLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
    this->addChild(_touchInfoLabel, 1);

    return true;
}

void CardLayer::_initLayers()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _playfieldLayer = LayerColor::create(Color4B(204, 153, 51, 255), 1080, 1500);
    _playfieldLayer->setPosition(Vec2(0, 580));
    this->addChild(_playfieldLayer, 0);

    _stackLayer = LayerColor::create(Color4B(128, 0, 128, 255), 1080, 580);
    _stackLayer->setPosition(Vec2(0, 0));
    this->addChild(_stackLayer, 0);
}

void CardLayer::_setupUI()
{
    _setPlayfieldCards();
    _setStackfieldCards();
    _adjustStackfieldPosition();
}

void CardLayer::_setPlayfieldCards()
{
    _playfieldLayer->removeAllChildren();
    _playfieldCardsA.clear();
    _playfieldCardsB.clear();

    // A-zone card logic: later cards cover earlier ones (top of stack on top)
    auto stackACards = _controller->getPlayfieldCardsA();
    std::stack<CardModel*> tempStackA = stackACards;
    int zOrder = tempStackA.size();

    while (!tempStackA.empty()) {
        const auto* cardModel = tempStackA.top();
        auto card = CardSprite::create(cardModel);
        card->setPosition(cardModel->position);
        _playfieldLayer->addChild(card, zOrder--); // Top card gets highest z-order
        _playfieldCardsA.push_back(card);
        tempStackA.pop();
    }

    // B-zone card logic: earlier cards cover later ones (top of stack at bottom)
    auto stackBCards = _controller->getPlayfieldCardsB();
    std::stack<CardModel*> tempStackB = stackBCards;

    while (!tempStackB.empty()) {
        const auto* cardModel = tempStackB.top();
        auto card = CardSprite::create(cardModel);
        card->setPosition(cardModel->position);
        _playfieldLayer->addChild(card); // Default z-order, added in order, so top of stack is visually at bottom
        _playfieldCardsB.push_back(card);
        tempStackB.pop();
    }
}

void CardLayer::_setStackfieldCards()
{
    // A-zone card logic
    auto stackACards = _controller->getStackCardsA();
    std::stack<CardModel*> tempStackA = stackACards;
    int zOrder = tempStackA.size();

    while (!tempStackA.empty()) {
        const auto* cardModel = tempStackA.top();
        auto card = CardSprite::create(cardModel);
        _partAStackCards.push_back(card);
        _stackLayer->addChild(card, zOrder--);
        tempStackA.pop();
    }
    std::reverse(_partAStackCards.begin(), _partAStackCards.end());

    // B-zone card logic
    auto stackBCards = _controller->getStackCardsB();
    std::stack<CardModel*> tempStackB = stackBCards;

    if (!tempStackB.empty()) {
        const auto* cardModel = tempStackB.top();
        _partBStackCard = CardSprite::create(cardModel);
        _stackLayer->addChild(_partBStackCard);
    }
}

void CardLayer::_adjustStackfieldPosition()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float cardWidth = 0;
    if (_partBStackCard) {
        cardWidth = _partBStackCard->getContentSize().width;
    } else if (!_partAStackCards.empty()) {
        cardWidth = _partAStackCards[0]->getContentSize().width;
    }

    if (cardWidth == 0) return; // No cards to layout

    float yPosition = _stackLayer->getContentSize().height / 2;
    float gap = 40.0f;

    float totalWidthA = 0;
    float spacingA = 0;
    int numPartACards = _partAStackCards.size();
    if (numPartACards > 0) {
        spacingA = cardWidth * 0.8f;
        totalWidthA = cardWidth + (numPartACards - 1) * spacingA;
    }

    float totalWidthB = (_partBStackCard != nullptr) ? cardWidth : 0;

    float totalLayoutWidth = totalWidthA + totalWidthB + (numPartACards > 0 && _partBStackCard != nullptr ? gap : 0);
    float startX = (visibleSize.width - totalLayoutWidth) / 2.0f;

    float currentX = startX;
    for (int i = 0; i < numPartACards; ++i)
    {
        _partAStackCards[i]->setPosition(Vec2(currentX + cardWidth / 2.0f, yPosition));
        currentX += spacingA;
    }

    if (_partBStackCard != nullptr) {
        currentX = startX + totalWidthA + (numPartACards > 0 ? gap : 0);
        _partBStackCard->setPosition(Vec2(currentX + cardWidth / 2.0f, yPosition));
    }
}

void CardLayer::_refreshStackLayer()
{
    _stackLayer->removeAllChildren();
    _partAStackCards.clear();
    _partBStackCard = nullptr;
    _setStackfieldCards();
    _adjustStackfieldPosition();
}

bool CardLayer::onTouchBegan(Touch* touch, Event* event)
{
    if (_handlePlayfieldTouch(touch)) {
        return true;
    }

    if (_handleStackLayerTouch(touch)) {
        return true;
    }

    _touchInfoLabel->setString("");
    return false;
}

bool CardLayer::_handlePlayfieldTouch(cocos2d::Touch* touch)
{
    auto check_touch = [this, touch](const std::vector<CardSprite*>& cards) -> bool {
        Vec2 location = touch->getLocation();
        for (auto cardSprite : cards)
        {
            if (cardSprite->getBoundingBox().containsPoint(location))
            {
                const CardModel* cardModel = cardSprite->getCardModel();
                const char* suit_name[] = {"Club", "Diamond", "Heart", "Spade"};
                const char* face_name[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
                std::string info = "Touched Card: " + std::string(face_name[(int)cardModel->face]) + " of " + std::string(suit_name[(int)cardModel->suit]);
                _touchInfoLabel->setString(info);
                return true;
            }
        }
        return false;
    };

    if (check_touch(_playfieldCardsA)) return true;
    if (check_touch(_playfieldCardsB)) return true;

    return false;
}

bool CardLayer::_handleStackLayerTouch(cocos2d::Touch* touch)
{
    Vec2 location = touch->getLocation();
    if (!_partAStackCards.empty()) {
        auto rightmostCard = _partAStackCards.back();
        if (rightmostCard->getBoundingBox().containsPoint(location))
        {
            _controller->moveCardFromAtoB();
            _refreshStackLayer();
            return true;
        }
    }
    return false;
}

void CardLayer::onTouchMoved(Touch* touch, Event* event){}

void CardLayer::onTouchEnded(Touch* touch, Event* event){}

void CardLayer::onTouchCancelled(Touch* touch, Event* event){}