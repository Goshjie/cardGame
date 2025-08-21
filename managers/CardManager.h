#ifndef __CARD_MANAGER_H__
#define __CARD_MANAGER_H__

#include "cocos2d.h"
#include "models/CardModel.h"
#include "json/document.h"
#include <stack>

/**
 * @class CardManager
 * @brief Manages the business logic for creating and distributing cards.
 *
 * This class takes raw card data, creates CardModel objects, and assigns them to the
 * appropriate play areas (playfield and stack).
 */
class CardManager
{
public:
    /**
     * @brief Gets the singleton instance of the CardManager.
     * @return The singleton CardManager instance.
     */
    static CardManager* getInstance();

    /**
     * @brief Processes the raw card data to create and distribute card models.
     *
     * @param document The raw card data from CardService.
     * @param playfieldCards A vector to be populated with playfield cards.
     * @param stackCardsA A stack to be populated with stack cards for area A.
     * @param stackCardsB A stack to be populated with stack cards for area B.
     */
    void processCardData(const rapidjson::Document& document, std::vector<CardModel*>& playfieldCards, std::stack<CardModel*>& stackCardsA, std::stack<CardModel*>& stackCardsB);

private:
    /**
     * @brief Private constructor for the singleton pattern.
     */
    CardManager();

    static CardManager* _instance;
};

#endif // __CARD_MANAGER_H__