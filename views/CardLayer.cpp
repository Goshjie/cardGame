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

    _rollbackButton = cocos2d::ui::Button::create();
    _rollbackButton->setTitleText("Rollback");
    _rollbackButton->setTitleFontSize(48);
    _rollbackButton->addClickEventListener(CC_CALLBACK_1(CardLayer::_onRollbackClicked, this));
    this->addChild(_rollbackButton);

    _adjustStackfieldPosition();
}

void CardLayer::_setPlayfieldCards()
{
    _playfieldLayer->removeAllChildren();
    _playfieldCardsA.clear();
    _playfieldCardsB.clear();

    // Logic for Stack A (newer cards on top)
    std::stack<CardModel*> tempStackA = _controller->getPlayfieldCardsA();
    int zOrderA = tempStackA.size();
    while (!tempStackA.empty()) {
        const auto* cardModel = tempStackA.top();
        auto card = CardSprite::create(cardModel);
        card->setPosition(cardModel->position);
        _playfieldLayer->addChild(card, zOrderA--);
        _playfieldCardsA.push_back(card);
        tempStackA.pop();
    }

    // Logic for Stack B (newer cards on top)
    std::stack<CardModel*> tempStackB = _controller->getPlayfieldCardsB();
    int zOrderB = tempStackB.size();
    while (!tempStackB.empty()) {
        const auto* cardModel = tempStackB.top();
        auto card = CardSprite::create(cardModel);
        card->setPosition(cardModel->position);
        _playfieldLayer->addChild(card, zOrderB--);
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
    float yPosition = _stackLayer->getContentSize().height / 2;
    float gap = 40.0f;

    // --- Calculate widths of all components ---
    float cardWidth = 0;
    if (!_partAStackCards.empty()) {
        cardWidth = _partAStackCards[0]->getContentSize().width;
    } else if (_partBStackCard) {
        cardWidth = _partBStackCard->getContentSize().width;
    }

    float totalWidthA = 0;
    float spacingA = 0;
    if (!_partAStackCards.empty()) {
        spacingA = cardWidth * 0.3f; // Let cards overlap
        totalWidthA = cardWidth + (_partAStackCards.size() - 1) * spacingA;
    }

    float totalWidthB = _partBStackCard ? cardWidth : 0;
    float buttonWidth = _rollbackButton ? _rollbackButton->getContentSize().width : 0;

    // --- Calculate total layout width for centering ---
    std::vector<float> widths;
    if (totalWidthA > 0) widths.push_back(totalWidthA);
    if (totalWidthB > 0) widths.push_back(totalWidthB);
    if (buttonWidth > 0) widths.push_back(buttonWidth);

    float totalLayoutWidth = 0;
    for(size_t i = 0; i < widths.size(); ++i) {
        totalLayoutWidth += widths[i];
    }
    if (widths.size() > 1) {
        totalLayoutWidth += (widths.size() - 1) * gap;
    }

    // --- Position elements ---
    float currentX = (visibleSize.width - totalLayoutWidth) / 2.0f;

    // Position stack A
    if (!_partAStackCards.empty()) {
        float cardAnchorX = currentX;
        for (auto& card : _partAStackCards) {
            card->setPosition(Vec2(cardAnchorX + cardWidth / 2.0f, yPosition));
            cardAnchorX += spacingA;
        }
        currentX += totalWidthA + gap;
    }

    // Position stack B
    if (_partBStackCard) {
        _partBStackCard->setPosition(Vec2(currentX + cardWidth / 2.0f, yPosition));
        currentX += totalWidthB + gap;
    }

    // Position back button
    if (_rollbackButton) {
        _rollbackButton->setPosition(Vec2(currentX + buttonWidth / 2.0f, yPosition));
    }
}

void CardLayer::_refreshPlayfieldLayer()
{
    _playfieldLayer->removeAllChildren();
    _playfieldCardsA.clear();
    _playfieldCardsB.clear();
    _setPlayfieldCards();
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
    Vec2 location = _playfieldLayer->convertToNodeSpace(touch->getLocation());

    auto check_and_move_card = [this](CardSprite* cardSprite, const Vec2& location) -> bool {
        if (cardSprite && cardSprite->getBoundingBox().containsPoint(location))
        {
            if (_controller->tryMoveCardFromPlayfieldToStack(const_cast<CardModel*>(cardSprite->getCardModel())))
            {
                _refreshPlayfieldLayer();
                _refreshStackLayer();
                return true;
            }
        }
        return false;
    };

    // Check the top card of stack A
    if (!_playfieldCardsA.empty()) {
        if (check_and_move_card(_playfieldCardsA.front(), location)) {
            return true;
        }
    }

    // Check the top card of stack B
    if (!_playfieldCardsB.empty()) {
        if (check_and_move_card(_playfieldCardsB.front(), location)) {
            return true;
        }
    }

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

void CardLayer::_onRollbackClicked(cocos2d::Ref* sender)
{
    _controller->rollbackLastMove();
    _refreshPlayfieldLayer();
    _refreshStackLayer();
}

void CardLayer::onTouchMoved(Touch* touch, Event* event){}

void CardLayer::onTouchEnded(Touch* touch, Event* event){}

void CardLayer::onTouchCancelled(Touch* touch, Event* event){}