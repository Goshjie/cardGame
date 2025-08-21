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
     * @param model The data model for the card.
     * @return A pointer to the created CardSprite.
     */
    static CardSprite* create(const CardModel* model);

    /**
     * @brief Initializes the CardSprite instance.
     * @param model The data model for the card.
     * @return True if initialization is successful, false otherwise.
     */
    virtual bool init(const CardModel* model);

    /**
     * @brief Gets the card model associated with this sprite.
     * @return A pointer to the card model.
     */
    const CardModel* getCardModel() const { return _model; }

private:
    /// @brief The card's data model.
    const CardModel* _model;
};

#endif // __CARD_SPRITE_H__