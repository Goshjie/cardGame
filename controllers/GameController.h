#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"
#include "services/CardService.h"
#include "managers/CardManager.h"
#include <vector>
#include <stack>

class GameScene; // Forward declaration

/**
 * @class GameController
 * @brief Manages the game logic and coordinates between the model and the view.
 *
 * This class is responsible for loading the level data, setting up the cards,
 * and handling the game's business logic. It communicates with the GameScene
 * to update the view.
 */
class GameController
{
public:
    GameController();

    /**
     * @brief Initializes the controller, loading card data.
     */
    void init();

    /**
     * @brief Gets the cards for the playfield.
     * @return A const reference to the vector of playfield card models.
     */
    const std::stack<CardModel*>& getPlayfieldCardsA() const;
    const std::stack<CardModel*>& getPlayfieldCardsB() const;

    /**
     * @brief Gets the cards for the stack area A (alternative area).
     * @return A const reference to the stack of card models for area A.
     */
    const std::stack<CardModel*>& getStackCardsA() const;

    /**
     * @brief Gets the cards for the stack area B (current hand area).
     * @return A const reference to the stack of card models for area B.
     */
    const std::stack<CardModel*>& getStackCardsB() const;

    /**
     * @brief Moves a card from stack A to stack B.
     */
    void moveCardFromAtoB();

private:
    std::stack<CardModel*> _playfieldCardsA; ///< Cards in the first playfield area.
    std::stack<CardModel*> _playfieldCardsB; ///< Cards in the second playfield area.
    std::stack<CardModel*> _stackCardsA;     ///< Cards in the stack area A.
    std::stack<CardModel*> _stackCardsB;     ///< Cards in the stack area B.
};