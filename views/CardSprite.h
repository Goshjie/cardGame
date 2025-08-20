#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"

class CardSprite : public cocos2d::Sprite
{
public:
    static CardSprite* create(CardFaceType face, CardSuitType suit);
    virtual bool init(CardFaceType face, CardSuitType suit);

private:
    CardFaceType _face;
    CardSuitType _suit;
};