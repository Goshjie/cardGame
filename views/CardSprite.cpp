#include "views/CardSprite.h"

USING_NS_CC;

CardSprite* CardSprite::create(CardFaceType face, CardSuitType suit)
{
    CardSprite *pRet = new(std::nothrow) CardSprite();
    if (pRet && pRet->init(face, suit))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool CardSprite::init(CardFaceType face, CardSuitType suit)
{
    if (!Sprite::initWithFile("card_general.png"))
    {
        return false;
    }

    _face = face;
    _suit = suit;

    const char* suit_name[] = {"club", "diamond", "heart", "spade"};
    auto suitSprite = Sprite::create(StringUtils::format("suits/%s.png", suit_name[(int)suit]));
    suitSprite->setPosition(Vec2(30, 100));
    this->addChild(suitSprite);

    const char* face_name[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    const char* color_name[] = {"black", "red"};
    int color_idx = ((int)suit == (int)CardSuitType::CST_DIAMONDS || (int)suit == (int)CardSuitType::CST_HEARTS) ? 1 : 0;

    auto faceSprite = Sprite::create(StringUtils::format("number/small_%s_%s.png", color_name[color_idx], face_name[(int)face]));
    faceSprite->setPosition(Vec2(30, 150));
    this->addChild(faceSprite);

    return true;
}