#include "controllers/GameController.h"
#include "services/CardService.h"
#include "managers/CardManager.h"
#include "cocos2d.h"
#include <cmath>

USING_NS_CC;

GameController::GameController()
{
}

void GameController::init()
{
    rapidjson::Document cardData = CardService::getInstance()->getCardData();
    const auto& stack = cardData["Stack"].GetArray();
    if (stack.Size() > 0) {
        // The last card goes to stack B
        const auto& lastCardInfo = stack[stack.Size() - 1];
        _stackCardsB.push(new CardModel(
            static_cast<CardFaceType>(lastCardInfo["CardFace"].GetInt()),
            static_cast<CardSuitType>(lastCardInfo["CardSuit"].GetInt()),
            Vec2::ZERO
        ));

        // The rest of the cards go to stack A
        for (rapidjson::SizeType i = 0; i < stack.Size() - 1; ++i) {
            const auto& cardInfo = stack[i];
            _stackCardsA.push(new CardModel(
                static_cast<CardFaceType>(cardInfo["CardFace"].GetInt()),
                static_cast<CardSuitType>(cardInfo["CardSuit"].GetInt()),
                Vec2::ZERO
            ));
        }
    }

    // Process Playfield cards with new logic based on position
    const auto& playfield = cardData["Playfield"].GetArray();
    if (playfield.Size() > 0) {
        std::vector<CardModel*> allCards;
        for (rapidjson::SizeType i = 0; i < playfield.Size(); ++i) {
            const auto& cardInfo = playfield[i];
            allCards.push_back(new CardModel(
                static_cast<CardFaceType>(cardInfo["CardFace"].GetInt()),
                static_cast<CardSuitType>(cardInfo["CardSuit"].GetInt()),
                Vec2(cardInfo["Position"]["x"].GetFloat(), cardInfo["Position"]["y"].GetFloat())
            ));
        }

        if (!allCards.empty()) {
            _playfieldCardsA.push(allCards[0]);
            for (size_t i = 1; i < allCards.size(); ++i) {
                if (std::abs(allCards[i]->position.x - allCards[i-1]->position.x) == 50) {
                    _playfieldCardsA.push(allCards[i]);
                } else {
                    // A large jump in x-distance indicates the start of the second stack
                    for(size_t j = i; j < allCards.size(); ++j) {
                        _playfieldCardsB.push(allCards[j]);
                    }
                    break;
                }
            }
        }
    }
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
        CardModel* card = _stackCardsA.top();
        card->owner = CardOwner::STACK_A;
        _stackCardsB.push(card);
        _stackCardsA.pop();
    }
}

bool GameController::tryMoveCardFromPlayfieldToStack(CardModel* card)
{
    if (_stackCardsB.empty()) {
        return false; // No card in stack B to compare with
    }

    CardModel* topCard = _stackCardsB.top();
    if (std::abs(static_cast<int>(card->face) - static_cast<int>(topCard->face)) == 1) {
        // Found a match, now move the card
        std::stack<CardModel*> tempStack;
        bool found = false;

        // Search in playfield A
        while (!_playfieldCardsA.empty()) {
            if (_playfieldCardsA.top() == card) {
                found = true;
                card->owner = CardOwner::PLAYFIELD_A;
                _playfieldCardsA.pop();
                break;
            }
            tempStack.push(_playfieldCardsA.top());
            _playfieldCardsA.pop();
        }
        while (!tempStack.empty()) {
            _playfieldCardsA.push(tempStack.top());
            tempStack.pop();
        }

        if (found) {
            _stackCardsB.push(card);
            return true;
        }

        // Search in playfield B
        while (!_playfieldCardsB.empty()) {
            if (_playfieldCardsB.top() == card) {
                found = true;
                card->owner = CardOwner::PLAYFIELD_B;
                _playfieldCardsB.pop();
                break;
            }
            tempStack.push(_playfieldCardsB.top());
            _playfieldCardsB.pop();
        }
        while (!tempStack.empty()) {
            _playfieldCardsB.push(tempStack.top());
            tempStack.pop();
        }

        if (found) {
            _stackCardsB.push(card);
            return true;
        }
    }

    return false;
}

void GameController::rollbackLastMove()
{
    if (!_stackCardsB.empty()) {
        CardModel* card = _stackCardsB.top();
        switch (card->owner) {
            case CardOwner::PLAYFIELD_A:
                _playfieldCardsA.push(card);
                break;
            case CardOwner::PLAYFIELD_B:
                _playfieldCardsB.push(card);
                break;
            case CardOwner::STACK_A:
                _stackCardsA.push(card);
                break;
            default:
                // Should not happen
                break;
        }
        _stackCardsB.pop();
    }
}