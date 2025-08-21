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
     * @brief Constructor for the CardManager.
     */
    CardManager();

    /**
     * @brief Processes the raw card data to create and distribute card models.
     *
     * @param document The raw card data from CardService.
     * @param playfieldCardsA A stack to be populated with playfield cards for area A.
     * @param playfieldCardsB A stack to be populated with playfield cards for area B.
     * @param stackCardsA A stack to be populated with stack cards for area A.
     * @param stackCardsB A stack to be populated with stack cards for area B.
     */
    void processCardData(const rapidjson::Document& document, std::stack<CardModel*>& playfieldCardsA, std::stack<CardModel*>& playfieldCardsB, std::stack<CardModel*>& stackCardsA, std::stack<CardModel*>& stackCardsB);

private:

};

#endif // __CARD_MANAGER_H__