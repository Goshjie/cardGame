#include "services/CardService.h"
#include "cocos2d.h"

USING_NS_CC;

CardService* CardService::_instance = nullptr;

CardService* CardService::getInstance()
{
    if (!_instance)
    {
        _instance = new CardService();
    }
    return _instance;
}

CardService::CardService()
{
}

rapidjson::Document CardService::getCardData()
{
    rapidjson::Document document;
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename("res/level_0.json");

    if (!FileUtils::getInstance()->isFileExist(fullPath))
    {
        CCLOGERROR("cards.json not found at path: %s", fullPath.c_str());
        return document;
    }

    std::string content = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    if (content.empty())
    {
        CCLOGERROR("cards.json is empty.");
        return document;
    }

    document.Parse(content.c_str());

    if (document.HasParseError())
    {
        CCLOGERROR("JSON parse error: %s", document.GetParseError());
    }

    return document;
}