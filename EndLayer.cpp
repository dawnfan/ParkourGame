#include "EndLayer.h"
#include "HelloWorldScene.h"


Scene *EndLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = EndLayer::create();
	scene->addChild(layer);
	return scene;
}

bool EndLayer::init(){
	if (!Layer::init()) {
		return false;
	}
	//背景
	Size screenSize = Director::sharedDirector()->getVisibleSize();
	end = Sprite::create("main_game/bg_fail.png");
	end->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	this->addChild(end);
	//关卡
	level = LabelTTF::create("0", "fonts/MSYH.TTF", 60);
	level->setPosition(screenSize.width/2+80,screenSize.height-58);
	level->setColor(Color3B(232,150,121));
	end->addChild(level);
	//指示语
	prompt = LabelTTF::create("YOU ARE STOPPED HERE !","fonts/MSYH.TTF",60);
	prompt->setPosition(screenSize.width/2,screenSize.height-138);
	prompt->setColor(Color3B(232,150,121));
	end->addChild(prompt);
	//分数
	end_score = LabelTTF::create("distance", "fonts/JOKERMAN.TTF", 48);
	end_score->setPosition(screenSize.width/2+100,screenSize.height/2+100);
	end_score->setColor(Color3B(139, 248, 178));
	end->addChild(end_score);
	target = LabelTTF::create("target", "fonts/JOKERMAN.TTF", 48);
	target->setPosition(screenSize.width/2+100,screenSize.height/2-40);
	target->setColor(Color3B(255,209,27));
	end->addChild(target);

	//响应键盘消息
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(EndLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

//根据win的真假设置背景和按钮
void EndLayer::setNext(bool win , unsigned n_lev=0 , unsigned n_tar=0) {
	Size screenSize = Director::sharedDirector()->getVisibleSize();
	MenuItemImage* btn_to;
	if (win)
	{
		end->setTexture(TextureCache::sharedTextureCache()->addImage("main_game/bg_success.png"));
		btn_to = MenuItemImage::create(
			"main_game/next1.png",
			"main_game/next2.png",
			CC_CALLBACK_1(EndLayer::NextFunc, this));
		this->n_level = n_lev;
		this->n_target = n_tar;
	}
	else{
		btn_to = MenuItemImage::create(
			"main_game/retry1.png",
			"main_game/retry2.png",
			CC_CALLBACK_1(EndLayer::RetryFunc, this));
	}

	btn_to->setPosition(screenSize.width/2 - btn_to->getContentSize().width,btn_to->getContentSize().height/2);
	//按钮Home
	auto btn_home = MenuItemImage::create(
		"main_game/home1.png",
		"main_game/home2.png",
		CC_CALLBACK_1(EndLayer::ReturnToMainFunc, this));
	btn_home->setPosition(screenSize.width/2 + btn_home->getContentSize().width,btn_home->getContentSize().height/2);

	auto menu = Menu::create(btn_to,btn_home,NULL);
	menu->setPosition(0,0);
	end->addChild(menu);
}

void EndLayer::NextFunc(cocos2d::Ref* pSender){
	//下一关
	Scene* newScene = PlayLayer::createScene();
	PlayLayer* layer = (PlayLayer*)(newScene->getChildByTag(13));
	//设定下一关的初始值
	layer->setLevel(this->n_level);
	layer->setTarget(this->n_target);
	layer->getHaze()->setOpacity(layer->getHaze()->getOpacity() - 30);
	layer->setPhyWorld(newScene->getPhysicsWorld());
	Director::sharedDirector()->replaceScene(newScene);
}

void EndLayer::RetryFunc(cocos2d::Ref* pSender){
	CCDirector::sharedDirector()->replaceScene(PlayLayer::createScene());
}

void EndLayer::ReturnToMainFunc(cocos2d::Ref* pSender){
	auto newScene = HelloWorld::createScene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.2,newScene));
}

//响应安卓返回键
void EndLayer::onKeyReleased(EventKeyboard::KeyCode keycode,Event* event)
{
	if(keycode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		Director::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.2,HelloWorld::createScene()));
	}
}