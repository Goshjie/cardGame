#include "controllers/GameController.h"
#include "services/CardService.h"
#include "managers/CardManager.h"
#include "cocos2d.h"
#include <cmath>

USING_NS_CC;

GameController::GameController()
{
    _cardManager = new CardManager();
}

void GameController::init()
{
    rapidjson::Document cardData = CardService::getInstance()->getCardData();
    _cardManager->processCardData(cardData, _playfieldCardsA, _playfieldCardsB, _stackCardsA, _stackCardsB);
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

bool GameController::_findAndMoveCard(std::stack<CardModel*>& playfield, CardModel* card, CardOwner owner)
{
    std::stack<CardModel*> tempStack;
    bool found = false;

    while (!playfield.empty()) {
        if (playfield.top() == card) {
            found = true;
            card->owner = owner;
            playfield.pop();
            break;
        }
        tempStack.push(playfield.top());
        playfield.pop();
    }

    // Restore the remaining cards back to the playfield
    while (!tempStack.empty()) {
        playfield.push(tempStack.top());
        tempStack.pop();
    }

    if (found) {
        _stackCardsB.push(card);
    }

    return found;
}



bool GameController::tryMoveCardFromPlayfieldToStack(CardModel* card)
{
    if (_stackCardsB.empty()) {
        return false; // No card in stack B to compare with
    }

    CardModel* topCard = _stackCardsB.top();
    if (std::abs(static_cast<int>(card->face) - static_cast<int>(topCard->face)) == 1) {
        // Try to find and move the card from playfield A first
        if (_findAndMoveCard(_playfieldCardsA, card, CardOwner::PLAYFIELD_A)) {
            return true;
        }
        
        // If not found in playfield A, try playfield B
        return _findAndMoveCard(_playfieldCardsB, card, CardOwner::PLAYFIELD_B);
    }

    return false;
}

void GameController::rollbackLastMove()
{
    if (!_stackCardsB.empty()) {
        CardModel* card = _stackCardsB.top();
        if (card->owner == CardOwner::NONE) {
            // This is an initial card, do not roll back.
            return;
        }
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