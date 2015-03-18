#include "EndLayer.h"
#include "HelloWorldScene.h"
#include "GameWin.h"

EndLayer::EndLayer():
	hazeCount(0),
	catchedDrop(false)
{}

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
	//����
	Size screenSize = Director::sharedDirector()->getVisibleSize();
	end = Sprite::create("main_game/bg_fail.png");
	end->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	this->addChild(end);
	//�ؿ�
	level = LabelTTF::create("0", "fonts/msyh.ttf", 60);
	level->setPosition(screenSize.width/2+80,screenSize.height-58);
	level->setColor(Color3B(232,150,121));
	end->addChild(level);
	//ָʾ��
	prompt = LabelTTF::create("YOU ARE STOPPED HERE !","fonts/msyh.ttf",60);
	prompt->setPosition(screenSize.width/2,screenSize.height-138);
	prompt->setColor(Color3B(232,150,121));
	end->addChild(prompt);
	//����
	end_score = LabelTTF::create("distance", "fonts/jokerman.ttf", 48);
	end_score->setPosition(screenSize.width/2+100,screenSize.height/2+100);
	end_score->setColor(Color3B(139, 248, 178));
	end->addChild(end_score);
	target = LabelTTF::create("target", "fonts/jokerman.ttf", 48);
	target->setPosition(screenSize.width/2+100,screenSize.height/2-40);
	target->setColor(Color3B(255,209,27));
	end->addChild(target);

	//��Ӧ������Ϣ
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(EndLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

//����win��������ñ����Ͱ�ť
void EndLayer::setNext(bool win , unsigned n_lev=0 , unsigned n_tar=0) {
	Size screenSize = Director::sharedDirector()->getVisibleSize();
	MenuItemImage* btn_to;
	if (win)
	{
		//����hazeUpdate
		this->schedule(schedule_selector(EndLayer::hazeUpdate),0.02,14,0.05);
		end->setTexture(TextureCache::sharedTextureCache()->addImage("main_game/bg_success.png"));
		btn_to = MenuItemImage::create(
			"main_game/next1.png",
			"main_game/next2.png",
			CC_CALLBACK_1(EndLayer::NextFunc, this));
		this->n_level = n_lev;
		this->n_target = n_tar;
		//ʤ�����治��Ҫ������Ŀ��
		end->removeChild(target,false);
		end->removeChild(end_score,false);
		//this->target->setString("100");
		//this->end_score->setString("100");
		//��ʾhaze
		hazeCount = 15;
		auto h=n_level<7?100-(n_level-1)*15:0;
		haze = LabelTTF::create(CCString::createWithFormat("%d",h+hazeCount)->getCString(),"fonts/scoreboard.ttf",80);
		haze->setColor(Color3B::RED);
		haze->setPosition(screenSize.width/2+50,screenSize.height/2+50);
		end->addChild(haze);
	}
	else{
		btn_to = MenuItemImage::create(
			"main_game/retry1.png",
			"main_game/retry2.png",
			CC_CALLBACK_1(EndLayer::RetryFunc, this));
	}

	btn_to->setPosition(screenSize.width/2 - btn_to->getContentSize().width,btn_to->getContentSize().height/2);
	//��ťHome
	auto btn_home = MenuItemImage::create(
		"main_game/home1.png",
		"main_game/home2.png",
		CC_CALLBACK_1(EndLayer::ReturnToMainFunc, this));
	btn_home->setPosition(screenSize.width/2 + btn_home->getContentSize().width,btn_home->getContentSize().height/2);

	auto menu = Menu::create(btn_to,btn_home,NULL);
	menu->setPosition(0,0);
	end->addChild(menu);
}

void EndLayer::hazeUpdate(float dt){
	if (this->hazeCount > 0)
	{
		hazeCount--;
		haze->setString(CCString::createWithFormat("%d",100-(n_level-1)*15+hazeCount)->getCString());
	}
	else
	{
		return;
	}
}

void EndLayer::NextFunc(cocos2d::Ref* pSender){
	//��һ��
	int nextHaze = 210 - 30 * (n_level - 1);
	if (nextHaze <= 0){
		Scene* newScene = GameWin::createScene();
		GameWin* layer = (GameWin*)(newScene->getChildByTag(13));
		Director::sharedDirector()->replaceScene(newScene);
	}
	else{
		Scene* newScene = PlayLayer::createScene();
		PlayLayer* layer = (PlayLayer*)(newScene->getChildByTag(13));
		//�趨��һ�صĳ�ʼֵ
		if (catchedDrop){
			layer->getProbar()->setPercentage(100);
		}
		layer->setLevel(this->n_level);
		layer->setTarget(this->n_target);
		layer->getHaze()->setOpacity(nextHaze);
		layer->setPhyWorld(newScene->getPhysicsWorld());
		Director::sharedDirector()->replaceScene(newScene);
	}
}

void EndLayer::RetryFunc(cocos2d::Ref* pSender){
	CCDirector::sharedDirector()->replaceScene(PlayLayer::createScene());
}

void EndLayer::ReturnToMainFunc(cocos2d::Ref* pSender){
	auto newScene = HelloWorld::createScene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.2,newScene));
}

//��Ӧ��׿���ؼ�
void EndLayer::onKeyReleased(EventKeyboard::KeyCode keycode,Event* event)
{
	if(keycode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		Director::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.2,HelloWorld::createScene()));
	}
}

void EndLayer::setCatchedDrop(){
	this->catchedDrop = true;
}