#include "Manager.h"
#include "PlayLayer.h"

USING_NS_CC;


bool Manager::init(){
	if (!Node::init()){
		return false;
	}
	frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("hazefight.plist", "hazefight.png");
	createBarrier();
	createBug();
	createCoin();
	this->height = 60;
	this->scheduleUpdate();
	return true;
}

void Manager::createCoin(){
	for (int i = 0; i < 5; ++i){
		auto coin = Base::create();
		coin->sprite = Sprite::createWithSpriteFrameName("waterDrop0.png");
		coin->addChild(coin->sprite);
		coin->sprite->runAction(coin->createAnimate(frameCache,4,"waterDrop"));
		coin->initBody(0);
		coin->setVisible(false);
		coin->setTag(coinTag);
		this->addChild(coin);
		this->m_coin.pushBack(coin);
	}
}

void Manager::createBarrier(){
	for (int i = 0; i < 5; ++i){
		auto barr = Base::create();
		//barr->sprite = Sprite::create("barrier.png");
		barr->sprite = Sprite::createWithSpriteFrameName("barrier.png");
		barr->addChild(barr->sprite);
		barr->initBody(0xFF);
		//barr->initBody(1);
		barr->setVisible(false);
		barr->setTag(barrTag);
		this->addChild(barr);
		this->m_barr.pushBack(barr);
	}
}

void Manager::update(float dt){
	int setNum = 0;
	for (Vector<Base*>::iterator iter = this->m_coin.begin(); iter != this->m_coin.end(); ++iter){
		Base* coin = *iter;
		//不在屏幕
		if (coin->getPositionX() < -coin->getContentSize().width ){
			coin->setVisible(false);
		}
		//要是不可见，要不就是不在屏幕里，要不就是被碰撞了
		if (!coin->isVisible()){
			setNum++;
		}
		//让金币移动
		coin->setPositionX(coin->getPositionX() - BACKGROUND_SPEED);
	}
	if (setNum == 5){
		int i = 0;
		float posX = WIDTH+400;
		float posY = height + CCRANDOM_0_1() * INTERVAL;
		for (Vector<Base*>::iterator iter = this->m_coin.begin(); iter != this->m_coin.end(); ++iter){
			Base* coin = *iter;
			//两个两个一起
			if (i < 2){
				posX += 100;//两个金币同一高度，间隔100
			}
			else{
				//重新重置位置 
				i = -1;
				posY = height + CCRANDOM_0_1() * INTERVAL;
				posX += WIDTH;
			}
			i++;
			coin->setVisible(true);
			coin->setPosition(posX, posY);
			if (i == 0){
				auto bug = *(m_bug.begin() + 1);
				bug->setPosition(posX, posY + 30);
				bug->setVisible(true);
			}
		}
	}
	setNum = 0;
	
	//障碍物
	for (Vector<Base*>::iterator iter = this->m_barr.begin(); iter != this->m_barr.end(); ++iter){
		Base* barr = *iter;
		if (barr->getPositionX() < -barr->getContentSize().width-100)
			barr->setVisible(false);
		if (!barr->isVisible())
			setNum++;
		//比背景移动的快一倍
		barr->setPositionX(barr->getPositionX() - (2+BACKGROUND_SPEED));
	}
	if (setNum == 5){
		float posX = WIDTH+400;
		float posY;
		int i = 0;
		for (Vector<Base*>::iterator iter = this->m_barr.begin(); iter != this->m_barr.end(); ++iter,++i){
			auto barr = *iter;
			posX += WIDTH;
			posY = height + CCRANDOM_0_1() * INTERVAL;
			if (i == 3){
				auto bug = *(m_bug.begin());
				bug->setPosition(posX, posY+160);
				bug->setVisible(true);
			}
			barr->setPosition(posX, posY);
			barr->setVisible(true);
		}
	}
	
	//bug移动
	for (Vector<Base*>::iterator iter = this->m_bug.begin(); iter != this->m_bug.end(); ++iter){
		auto bug = *iter;
		if (bug->getPositionX() < -bug->getContentSize().width - 100){
			bug->setVisible(false);
		}
		else
			bug->setPositionX(bug->getPositionX() - (4 + BACKGROUND_SPEED));
	}

	
}

void Manager::createBug(){
	for (int i = 0; i < 2; ++i){
		auto bug = Base::create();
		bug->sprite = Sprite::createWithSpriteFrameName("bug0.png");
		bug->addChild(bug->sprite);
		bug->sprite->runAction(bug->createAnimate(frameCache, 4, "bug"));
		auto phyBody = PhysicsBody::createBox(bug->sprite->getContentSize());
		phyBody->getShape(0)->setMaterial(PHYSICSBODY_MATERIAL_DEFAULT);
		phyBody->setCollisionBitmask(0x02);
		phyBody->setContactTestBitmask(1);
		phyBody->setRotationEnable(false);
		bug->setPhysicsBody(phyBody);
		bug->setVisible(false);
		bug->setTag(bugTag);
		this->addChild(bug);
		this->m_bug.pushBack(bug);
	}
}