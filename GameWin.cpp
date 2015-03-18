#include "GameWin.h"

#include "HelloWorldScene.h"
#include "About.h"

Scene *GameWin::createScene()
{
	auto scene = Scene::create();
	auto layer = GameWin::create();
	layer->setTag(13);
	scene->addChild(layer);
	return scene;
}

bool GameWin::init(){
	if (!Layer::init()) {
		return false;
	}
	Size screenSize = Director::sharedDirector()->getVisibleSize();
	auto GameWin = Sprite::create("success.png");
	GameWin->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
	this->addChild(GameWin);

	auto GameWin_home = MenuItemImage::create(
		"main_game/home1.png",
		"main_game/home2.png",
		CC_CALLBACK_1(GameWin::ReturnToMainFunc, this));
	GameWin_home->setPosition(screenSize.width - GameWin_home->getContentSize().width * 3 / 2 - 20, GameWin_home->getContentSize().height / 2);

	auto menu = Menu::create(GameWin_home, NULL);
	menu->setPosition(0, 0);
	GameWin->addChild(menu);
	//响应键盘消息
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(GameWin::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

void GameWin::ReturnToMainFunc(cocos2d::Ref* pSender){
	auto newScene = HelloWorld::createScene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.2, newScene));
}

//响应安卓返回键
void GameWin::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		auto newScene = HelloWorld::createScene();
		CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.2, newScene));
	}
}