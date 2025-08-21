#include "managers/CardManager.h"
#include "models/CardModel.h"
#include "cocos2d.h"
#include <stack>
#include <vector>
#include <algorithm>
#include <cmath>

USING_NS_CC;





#include "managers/CardManager.h"
#include "models/CardModel.h"
#include "cocos2d.h"
#include <stack>
#include <vector>
#include <algorithm>
#include <cmath>

USING_NS_CC;

CardManager::CardManager()
{
}

CardModel* CardManager::_createCardModelFromJson(const rapidjson::Value& cardInfo)
{
    return new CardModel(
        static_cast<CardFaceType>(cardInfo["CardFace"].GetInt()),
        static_cast<CardSuitType>(cardInfo["CardSuit"].GetInt()),
        cardInfo.HasMember("Position") ? cocos2d::Vec2(cardInfo["Position"]["x"].GetFloat(), cardInfo["Position"]["y"].GetFloat()) : cocos2d::Vec2::ZERO
    );
}

void CardManager::_processStackCards(const rapidjson::Value& stackData, std::stack<CardModel*>& stackCardsA, std::stack<CardModel*>& stackCardsB)
{
    if (stackData.IsArray() && stackData.Size() > 0)
    {
        const auto& stack = stackData.GetArray();
        stackCardsB.push(_createCardModelFromJson(stack[stack.Size() - 1]));

        for (rapidjson::SizeType i = 0; i < stack.Size() - 1; ++i)
        {
            stackCardsA.push(_createCardModelFromJson(stack[i]));
        }
    }
}

void CardManager::_processPlayfieldCards(const rapidjson::Value& playfieldData, std::stack<CardModel*>& playfieldCardsA, std::stack<CardModel*>& playfieldCardsB)
{
    if (playfieldData.IsArray() && playfieldData.Size() > 0)
    {
        const auto& playfield = playfieldData.GetArray();
        std::vector<CardModel*> allCards;
        for (const auto& cardInfo : playfield)
        {
            allCards.push_back(_createCardModelFromJson(cardInfo));
        }

        if (!allCards.empty())
        {
            playfieldCardsA.push(allCards[0]);
            for (size_t i = 1; i < allCards.size(); ++i)
            {
                if (std::abs(allCards[i]->position.x - allCards[i - 1]->position.x) == 50)
                {
                    playfieldCardsA.push(allCards[i]);
                }
                else
                {
                    for (size_t j = i; j < allCards.size(); ++j)
                    {
                        playfieldCardsB.push(allCards[j]);
                    }
                    break;
                }
            }
        }
    }
}

void CardManager::processCardData(const rapidjson::Document& document, std::stack<CardModel*>& playfieldCardsA, std::stack<CardModel*>& playfieldCardsB, std::stack<CardModel*>& stackCardsA, std::stack<CardModel*>& stackCardsB)
{
    if (document.HasMember("Stack"))
    {
        _processStackCards(document["Stack"], stackCardsA, stackCardsB);
    }

    if (document.HasMember("Playfield"))
    {
        _processPlayfieldCards(document["Playfield"], playfieldCardsA, playfieldCardsB);
    }
}
