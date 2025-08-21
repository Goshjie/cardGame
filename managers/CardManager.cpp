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

void CardManager::processCardData(const rapidjson::Document& document, std::stack<CardModel*>& playfieldCardsA, std::stack<CardModel*>& playfieldCardsB, std::stack<CardModel*>& stackCardsA, std::stack<CardModel*>& stackCardsB)
{
    // Process Stack cards
    if (document.HasMember("Stack") && document["Stack"].IsArray()) {
        const auto& stack = document["Stack"].GetArray();
        if (stack.Size() > 0) {
            // The last card goes to stack B
            const auto& lastCardInfo = stack[stack.Size() - 1];
            stackCardsB.push(new CardModel(
                static_cast<CardFaceType>(lastCardInfo["CardFace"].GetInt()),
                static_cast<CardSuitType>(lastCardInfo["CardSuit"].GetInt()),
                cocos2d::Vec2::ZERO
            ));

            // The rest of the cards go to stack A
            for (rapidjson::SizeType i = 0; i < stack.Size() - 1; ++i) {
                const auto& cardInfo = stack[i];
                stackCardsA.push(new CardModel(
                    static_cast<CardFaceType>(cardInfo["CardFace"].GetInt()),
                    static_cast<CardSuitType>(cardInfo["CardSuit"].GetInt()),
                    cocos2d::Vec2::ZERO
                ));
            }
        }
    }

    // Process Playfield cards
    if (document.HasMember("Playfield") && document["Playfield"].IsArray()) {
        const auto& playfield = document["Playfield"].GetArray();
        if (playfield.Size() > 0) {
            std::vector<CardModel*> allCards;
            for (rapidjson::SizeType i = 0; i < playfield.Size(); ++i) {
                const auto& cardInfo = playfield[i];
                allCards.push_back(new CardModel(
                    static_cast<CardFaceType>(cardInfo["CardFace"].GetInt()),
                    static_cast<CardSuitType>(cardInfo["CardSuit"].GetInt()),
                    cocos2d::Vec2(cardInfo["Position"]["x"].GetFloat(), cardInfo["Position"]["y"].GetFloat())
                ));
            }

            if (!allCards.empty()) {
                playfieldCardsA.push(allCards[0]);
                for (size_t i = 1; i < allCards.size(); ++i) {
                    if (std::abs(allCards[i]->position.x - allCards[i-1]->position.x) == 50) {
                        playfieldCardsA.push(allCards[i]);
                    } else {
                        // A large jump in x-distance indicates the start of the second stack
                        for(size_t j = i; j < allCards.size(); ++j) {
                            playfieldCardsB.push(allCards[j]);
                        }
                        break;
                    }
                }
            }
        }
    }
}
