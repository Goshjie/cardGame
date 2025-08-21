#include "managers/CardManager.h"
#include "models/CardModel.h"
#include "cocos2d.h"
#include <stack>
#include <vector>
#include <algorithm>

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

void CardManager::processCardData(const rapidjson::Document& document, std::stack<CardModel*>& playfieldCardsA, std::stack<CardModel*>& playfieldCardsB, std::stack<CardModel*>& stackCardsA, std::stack<CardModel*>& stackCardsB)
{
    // Process Playfield cards
    if (document.HasMember("Playfield") && document["Playfield"].IsArray()) {
        std::vector<CardModel*> tempPlayfieldCards;
        const rapidjson::Value& cards = document["Playfield"];
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
                tempPlayfieldCards.push_back(cardModel);
            }
        }

        std::sort(tempPlayfieldCards.begin(), tempPlayfieldCards.end(), [](CardModel* a, CardModel* b) {
            return a->position.x < b->position.x;
        });

        if (!tempPlayfieldCards.empty()) {
            std::stack<CardModel*>* currentStack = &playfieldCardsA;
            currentStack->push(tempPlayfieldCards[0]);
            for (size_t i = 1; i < tempPlayfieldCards.size(); ++i) {
                if (tempPlayfieldCards[i]->position.x - tempPlayfieldCards[i - 1]->position.x != 50) {
                    currentStack = (currentStack == &playfieldCardsA) ? &playfieldCardsB : &playfieldCardsA;
                }
                currentStack->push(tempPlayfieldCards[i]);
            }
        }
    }

    // Process Stack cards
    if (document.HasMember("Stack") && document["Stack"].IsArray()) {
        std::stack<CardModel*> tempStack;
        const rapidjson::Value& cards = document["Stack"];
        for (rapidjson::SizeType i = 0; i < cards.Size(); i++) {
            const rapidjson::Value& cardData = cards[i];
            if (cardData.IsObject() && cardData.HasMember("CardFace") && cardData.HasMember("CardSuit")) {
                CardModel* cardModel = new CardModel(
                    static_cast<CardFaceType>(cardData["CardFace"].GetInt()),
                    static_cast<CardSuitType>(cardData["CardSuit"].GetInt()),
                    cocos2d::Vec2::ZERO
                );
                tempStack.push(cardModel);
            }
        }

        if (!tempStack.empty()) {
            stackCardsB.push(tempStack.top());
            tempStack.pop();
        }
        while (!tempStack.empty()) {
            stackCardsA.push(tempStack.top());
            tempStack.pop();
        }
    }
}
