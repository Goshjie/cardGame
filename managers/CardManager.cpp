#include "managers/CardManager.h"
#include "models/CardModel.h"
#include "cocos2d.h"
#include <stack>

USING_NS_CC;

CardManager* CardManager::_instance = nullptr;

CardManager* CardManager::getInstance()
{
    if (!_instance)
    {
        _instance = new CardManager();
    }
    return _instance;
}

CardManager::CardManager()
{
}

void CardManager::processCardData(const rapidjson::Document& document, std::vector<CardModel*>& playfieldCards, std::stack<CardModel*>& stackCardsA, std::stack<CardModel*>& stackCardsB)
{
    auto process_playfield_cards = [&](const char* key, std::vector<CardModel*>& cards_vector) {
        if (document.HasMember(key) && document[key].IsArray()) {
            const rapidjson::Value& cards = document[key];
            for (rapidjson::SizeType i = 0; i < cards.Size(); i++) {
                const rapidjson::Value& cardData = cards[i];
                if (cardData.IsObject() && cardData.HasMember("CardFace") && cardData.HasMember("CardSuit")) {
                    cocos2d::Vec2 position = cocos2d::Vec2::ZERO;
                    if (cardData.HasMember("Position") && cardData["Position"].IsObject()) {
                        const rapidjson::Value& pos = cardData["Position"];
                        if (pos.HasMember("x") && pos.HasMember("y") && pos["x"].IsInt() && pos["y"].IsInt()) {
                            position.x = pos["x"].GetFloat();
                            position.y = pos["y"].GetFloat();
                        }
                    }
                    CardModel* cardModel = new CardModel(
                        static_cast<CardFaceType>(cardData["CardFace"].GetInt()),
                        static_cast<CardSuitType>(cardData["CardSuit"].GetInt()),
                        position
                    );
                    cards_vector.push_back(cardModel);
                } else {
                    CCLOGERROR("Card data is malformed for key: %s", key);
                }
            }
        } else {
            CCLOGERROR("JSON is missing array for key: %s", key);
        }
    };

    process_playfield_cards("Playfield", playfieldCards);

    const char* stackKey = "Stack";
    if (document.HasMember(stackKey) && document[stackKey].IsArray()) {
        const rapidjson::Value& stackData = document[stackKey];
        for (rapidjson::SizeType i = 0; i < stackData.Size(); ++i) {
            const rapidjson::Value& cardData = stackData[i];
            if (cardData.IsObject() && cardData.HasMember("CardFace") && cardData.HasMember("CardSuit")) {
                CardModel* cardModel = new CardModel(
                    static_cast<CardFaceType>(cardData["CardFace"].GetInt()),
                    static_cast<CardSuitType>(cardData["CardSuit"].GetInt()),
                    cocos2d::Vec2::ZERO
                );

                if (i == stackData.Size() - 1) {
                    stackCardsB.push(cardModel);
                } else {
                    stackCardsA.push(cardModel);
                }
            } else {
                CCLOGERROR("Card data is malformed for key: %s", stackKey);
            }
        }
    } else {
        CCLOGERROR("JSON is missing array for key: %s", stackKey);
    }
}
