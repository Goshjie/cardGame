#include "controllers/GameController.h"
#include "services/CardService.h"
#include "managers/CardManager.h"
#include "cocos2d.h"

USING_NS_CC;

GameController::GameController()
{
}

void GameController::init()
{
    rapidjson::Document cardData = CardService::getInstance()->getCardData();
    CardManager::getInstance()->processCardData(cardData, _playfieldCardsA, _playfieldCardsB, _stackCardsA, _stackCardsB);
}

const std::stack<CardModel*>& GameController::getPlayfieldCardsA() const
{
    return _playfieldCardsA;
}

const std::stack<CardModel*>& GameController::getPlayfieldCardsB() const
{
    return _playfieldCardsB;
}

const std::stack<CardModel*>& GameController::getStackCardsA() const
{
    return _stackCardsA;
}

const std::stack<CardModel*>& GameController::getStackCardsB() const
{
    return _stackCardsB;
}

void GameController::moveCardFromAtoB()
{
    if (!_stackCardsA.empty()) {
        _stackCardsB.push(_stackCardsA.top());
        _stackCardsA.pop();
    }
}