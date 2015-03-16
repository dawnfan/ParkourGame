#include "Popup.h"
#include "HelloWorldScene.h"
#include "About.h"

Scene *Popup::createScene()
{
	auto scene = Scene::create();
	auto layer = Popup::create();
	scene->addChild(layer);
	return scene;
}

bool Popup::init(){
	if (!Layer::init()) {
		return false;
	}

	Size screenSize = Director::sharedDirector()->getVisibleSize();
	auto popup = Sprite::create("main_game/bg_pop.png");
	popup->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	this->addChild(popup);

	auto popup_home = MenuItemImage::create(
		"main_game/home1.png",
		"main_game/home2.png",
		CC_CALLBACK_1(Popup::ReturnToMainFunc, this));
	popup_home->setPosition(screenSize.width/2+popup_home->getContentSize().width+20,
							screenSize.height/2-popup_home->getContentSize().height/4);
	auto popup_info = MenuItemImage::create(
		"menu/about1.png",
		"menu/about2.png",
		CC_CALLBACK_1(Popup::AboutFunc, this));
	popup_info->setPosition(screenSize.width/2-popup_info->getContentSize().width-20,
							screenSize.height/2-popup_info->getContentSize().height/4);
	auto popup_continue = MenuItemImage::create(
		"main_game/back1.png",
		"main_game/back2.png",
		CC_CALLBACK_1(Popup::BackFunc, this));
	popup_continue->setPosition(screenSize.width/2,screenSize.height/2-popup_continue->getContentSize().height/2);

	auto menu = Menu::create(popup_home,popup_info,popup_continue,NULL);
	menu->setPosition(0,0);
	popup->addChild(menu);

	//响应键盘消息
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(Popup::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

void Popup::BackFunc(cocos2d::Ref* pSender){
	Director::sharedDirector()->popScene();
}

void Popup::ReturnToMainFunc(cocos2d::Ref* pSender){
	auto newScene = HelloWorld::createScene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.2,newScene));
}

void Popup::AboutFunc(cocos2d::Ref* pSender){
	//ToDO：关于界面中返回键返回原处
	CCDirector::sharedDirector()->pushScene(GameAbout::createScene());
}

//响应安卓返回键
void Popup::onKeyReleased(EventKeyboard::KeyCode keycode,Event* event)
{
	if(keycode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		Director::sharedDirector()->popScene();
	}
}