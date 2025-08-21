#ifndef __CARD_LAYER_H__
#define __CARD_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "models/CardModel.h"
#include "views/CardSprite.h"
#include <vector>

class GameController;

class CardLayer : public cocos2d::Layer
{
public:
    static CardLayer* create(GameController* controller);
    virtual bool init(GameController* controller);

private:
    void _initLayers();
    void _setupUI();
    void _setPlayfieldCards();
    void _setStackfieldCards();
    void _adjustStackfieldPosition();
    void _refreshPlayfieldLayer();
    void _refreshStackLayer();

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    void _onBackClicked(cocos2d::Ref* sender);

    bool _handlePlayfieldTouch(cocos2d::Touch* touch);
    bool _handleStackLayerTouch(cocos2d::Touch* touch);

    GameController* _controller;
    cocos2d::Layer* _playfieldLayer;
    cocos2d::Layer* _stackLayer;
    std::vector<CardSprite*> _playfieldCardsA;
    std::vector<CardSprite*> _playfieldCardsB;
    std::vector<CardSprite*> _partAStackCards;
    CardSprite* _partBStackCard;
    cocos2d::ui::Button* _backButton;
    cocos2d::Label* _touchInfoLabel;
};

#endif // __CARD_LAYER_H__