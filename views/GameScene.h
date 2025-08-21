#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "views/CardLayer.h"

class GameController;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(GameScene);

private:
    GameController* _controller;
    CardLayer* _cardLayer;
};

#endif // __GAME_SCENE_H__
