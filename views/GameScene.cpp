#include "views/GameScene.h"
#include "controllers/GameController.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    _controller = new GameController();
    _controller->init();

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));

    float x = Director::getInstance()->getVisibleSize().width - closeItem->getContentSize().width/2;
    float y = Director::getInstance()->getVisibleSize().height - closeItem->getContentSize().height/2;
    closeItem->setPosition(Vec2(x,y));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    _cardLayer = CardLayer::create(_controller);
    this->addChild(_cardLayer);

    return true;
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

