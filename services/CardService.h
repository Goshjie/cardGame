#ifndef __CARD_SERVICE_H__
#define __CARD_SERVICE_H__

#include "cocos2d.h"
#include "json/document.h"

/**
 * @class CardService
 * @brief A service class responsible for loading and parsing card data from a JSON file.
 *
 * This class follows the singleton pattern to ensure that card data is loaded only once.
 * It reads the card configuration from a JSON file and provides it as a rapidjson::Document.
 */
class CardService
{
public:
    /**
     * @brief Gets the singleton instance of the CardService.
     * @return The singleton CardService instance.
     */
    static CardService* getInstance();

    /**
     * @brief Retrieves the card data as a rapidjson::Document.
     * @return A rapidjson::Document containing the parsed card data.
     */
    rapidjson::Document getCardData();

private:
    /**
     * @brief Private constructor for the singleton pattern.
     */
    CardService();

    static CardService* _instance;
};

#endif // __CARD_SERVICE_H__