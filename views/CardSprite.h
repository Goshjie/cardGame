#ifndef __CARD_SPRITE_H__
#define __CARD_SPRITE_H__

#include "cocos2d.h"
#include "models/CardModel.h"

/**
 * @class CardSprite
 * @brief The visual representation of a card in the game.
 *
 * This class is responsible for rendering a card's appearance based on its
 * face and suit. It combines various image assets to create the final
 * visual of the card.
 */
class CardSprite : public cocos2d::Sprite
{
public:
    /**
     * @brief Creates a new CardSprite instance.
     * @param face The face type of the card.
     * @param suit The suit type of the card.
     * @return A pointer to the created CardSprite.
     */
    static CardSprite* create(CardFaceType face, CardSuitType suit);

    /**
     * @brief Initializes the CardSprite instance.
     * @param face The face type of the card.
     * @param suit The suit type of the card.
     * @return True if initialization is successful, false otherwise.
     */
    virtual bool init(CardFaceType face, CardSuitType suit);

private:
    /// @brief The face type of the card.
    CardFaceType _face;

    /// @brief The suit type of the card.
    CardSuitType _suit;
};

#endif // __CARD_SPRITE_H__