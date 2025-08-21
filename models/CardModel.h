#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"

/**
 * @enum CardSuitType
 * @brief Defines the suit of a card (Clubs, Diamonds, Hearts, Spades).
 */
enum class CardSuitType 
{
    Clubs,
    Diamonds,
    Hearts,
    Spades
};

/**
 * @enum CardFaceType
 * @brief Defines the face value of a card (Ace, Two, ..., King).
 */
enum class CardFaceType 
{
    Ace,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
};

/**
 * @class CardModel
 * @brief Represents the data model for a single card.
 *
 * This class stores the essential properties of a card, including its face,
 * suit, and position. It is used by the GameController to manage the state
 * of each card in the game.
 */
class CardModel 
{
public:
    /**
     * @brief Constructor for the CardModel class.
     * @param face The face type of the card.
     * @param suit The suit type of the card.
     * @param position The initial position of the card.
     */
    CardModel(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position)
        : face(face), suit(suit), position(position) {}

    /// @brief The face type of the card.
    // 数值
    CardFaceType face;

    /// @brief The suit type of the card.
    // 花色
    CardSuitType suit;

    /// @brief The position of the card.
    cocos2d::Vec2 position;
};

#endif // __CARD_MODEL_H__