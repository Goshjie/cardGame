#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"
#include "json/document.h"
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
    const std::vector<CardModel>& getPlayfieldCards() const;

    /**
     * @brief Gets the cards for the stack.
     * @return A const reference to the vector of stack card models.
     */
    const std::vector<CardModel>& getStackCards() const;

private:
    /**
     * @brief Loads card data from the JSON file and processes it.
     */
    void _loadAndProcessCards();

    /**
     * @brief Sets up the playfield cards based on the provided data.
     * @param playfieldData The rapidjson value containing the playfield card data.
     */
    void _setupPlayfieldCards(const rapidjson::Value& playfieldData);

    /**
     * @brief Sets up the stack cards based on the provided data.
     * @param stackData The rapidjson value containing the stack card data.
     */
    void _setupStackCards(const rapidjson::Value& stackData);

    /// @brief A pointer to the view (GameScene).
    GameScene* _view;

    /// @brief The collection of card models for the playfield.
    std::vector<CardModel> _playfieldCards;

    /// @brief The collection of card models for the stack.
    std::vector<CardModel> _stackCards;
};