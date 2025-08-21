#include "controllers/GameController.h"
#include "views/GameScene.h"
#include "services/CardService.h"
#include "managers/CardManager.h"
#include "cocos2d.h"

USING_NS_CC;

GameController::GameController(GameScene* view) : _view(view)
{
}

void GameController::init()
{
    rapidjson::Document cardData = CardService::getInstance()->getCardData();
    CardManager::getInstance()->processCardData(cardData, _playfieldCards, _stackCards);
}

const std::vector<CardModel*>& GameController::getPlayfieldCards() const
{
    return _playfieldCards;
}

const std::vector<CardModel*>& GameController::getStackCards() const
{
    return _stackCards;
}