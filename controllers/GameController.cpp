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