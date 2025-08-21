#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "views/CardSprite.h"
#include <vector>

class GameController;

/**
 * @class GameScene
 * @brief The main view for the game, responsible for displaying the game state.
 *
 * This class renders the visual elements of the game, including the playfield
 * and the stack of cards. It communicates with the GameController to get the
 * necessary data for rendering.
 */
class GameScene : public cocos2d::Scene
{
public:
    /**
     * @brief Creates a new GameScene instance.
     * @return A pointer to the created GameScene.
     */
    static cocos2d::Scene* createScene();

    /**
     * @brief Initializes the GameScene instance.
     * @return True if initialization is successful, false otherwise.
     */
    virtual bool init();
    
    /**
     * @brief Callback function for the close menu item.
     * @param pSender A pointer to the sender of the callback.
     */
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

private:
    /**
     * @brief Initializes the layers for the playfield and stack.
     */
    void _initLayers();

    /**
     * @brief Sets up the user interface, including cards.
     */
    void _setupUI();

    void _setPlayfieldCards();
    void _setStackfieldCards();

    void _adjustStackfieldPosition(std::vector<CardSprite*>& partACards, CardSprite* partBCard);

    /// @brief Layer for the playfield cards.
    cocos2d::LayerColor* _playfieldLayer;

    /// @brief Layer for the stack cards.
    cocos2d::LayerColor* _stackLayer;

    /// @brief Pointer to the game controller.
    GameController* _controller;
};

#endif // __GAME_SCENE_H__
