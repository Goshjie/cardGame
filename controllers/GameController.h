#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"
#include "services/CardService.h"
#include "managers/CardManager.h"
#include <vector>

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
    /**
     * @brief Constructor for the GameController.
     * @param view A pointer to the GameScene view that this controller will manage.
     */
    GameController(GameScene* view);

    /**
     * @brief Initializes the controller, loading card data.
     */
    void init();

    /**
     * @brief Gets the cards for the playfield.
     * @return A const reference to the vector of playfield card models.
     */
    const std::vector<CardModel*>& getPlayfieldCards() const;

    /**
     * @brief Gets the cards for the stack.
     * @return A const reference to the vector of stack card models.
     */
    const std::vector<CardModel*>& getStackCards() const;

private:
    GameScene* _view;
    std::vector<CardModel*> _playfieldCards;
    std::vector<CardModel*> _stackCards;
};