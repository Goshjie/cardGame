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
    const char* face_name[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    const char* color_name[] = {"black", "red"};
    int color_idx = ((int)suit == (int)CardSuitType::CST_DIAMONDS || (int)suit == (int)CardSuitType::CST_HEARTS) ? 1 : 0;

    // 1. Big number in the center
    auto bigFaceSprite = Sprite::create(StringUtils::format("number/big_%s_%s.png", color_name[color_idx], face_name[(int)face]));
    if (bigFaceSprite)
    {
        bigFaceSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
        this->addChild(bigFaceSprite);
    }

    // 2. Small number in the top-left
    auto smallFaceSprite = Sprite::create(StringUtils::format("number/small_%s_%s.png", color_name[color_idx], face_name[(int)face]));
    if (smallFaceSprite)
    {
        smallFaceSprite->setPosition(Vec2(30, this->getContentSize().height - 30));
        this->addChild(smallFaceSprite);
    }

    // 3. Suit in the top-right
    auto suitSprite = Sprite::create(StringUtils::format("suits/%s.png", suit_name[(int)suit]));
    if (suitSprite)
    {
        suitSprite->setPosition(Vec2(this->getContentSize().width - 30, this->getContentSize().height - 30));
        this->addChild(suitSprite);
    }

    return true;
}
