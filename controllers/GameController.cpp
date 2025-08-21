#include "controllers/GameController.h"
#include "views/GameScene.h"
#include "cocos2d.h"

USING_NS_CC;

GameController::GameController(GameScene* view) : _view(view)
{
}

void GameController::init()
{
    _loadAndProcessCards();
}

const std::vector<CardModel>& GameController::getPlayfieldCards() const
{
    return _playfieldCards;
}

const std::vector<CardModel>& GameController::getStackCards() const
{
    return _stackCards;
}

void GameController::_loadAndProcessCards()
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename("res/level_0.json");
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(fullPath);

    rapidjson::Document doc;
    doc.Parse(contentStr.c_str());

    if (doc.HasMember("Playfield"))
    {
        _setupPlayfieldCards(doc["Playfield"]);
    }

    if (doc.HasMember("Stack"))
    {
        _setupStackCards(doc["Stack"]);
    }
}

void GameController::_setupPlayfieldCards(const rapidjson::Value& playfieldData)
{
    for (rapidjson::SizeType i = 0; i < playfieldData.Size(); i++)
    {
        const rapidjson::Value& cardInfo = playfieldData[i];
        _playfieldCards.emplace_back(
            static_cast<CardFaceType>(cardInfo["CardFace"].GetInt()),
            static_cast<CardSuitType>(cardInfo["CardSuit"].GetInt()),
            Vec2(cardInfo["Position"]["x"].GetFloat(), cardInfo["Position"]["y"].GetFloat())
        );
    }
}

void GameController::_setupStackCards(const rapidjson::Value& stackData)
{
    for (rapidjson::SizeType i = 0; i < stackData.Size(); i++)
    {
        const rapidjson::Value& cardInfo = stackData[i];
        _stackCards.emplace_back(
            static_cast<CardFaceType>(cardInfo["CardFace"].GetInt()),
            static_cast<CardSuitType>(cardInfo["CardSuit"].GetInt()),
            Vec2::ZERO
        );
    }
}