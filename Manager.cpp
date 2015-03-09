#include "Manager.h"

USING_NS_CC;


bool Manager::init(){
	if (!Node::init()){
		return false;
	}
	createCoin();
	createBarrier();
	this->scheduleUpdate();
	return true;
}

void Manager::createCoin(){
	for (int i = 0; i < 5; ++i){
		auto coin = Coin::create();
		coin->setVisible(false);
		coin->setTag(coinTag);
		this->addChild(coin);
		this->m_coinArr.pushBack(coin);
	}
}

void Manager::createBarrier(){
	for (int i = 0; i < 5; ++i){
		auto barr = Barrier::create();
		barr->setVisible(false);
		barr->setTag(barrTag);
		this->addChild(barr);
		this->m_barr.pushBack(barr);
	}
}

void Manager::update(float dt){
	int setNum = 0;
	for (Vector<Coin*>::iterator iter = this->m_coinArr.begin(); iter != this->m_coinArr.end(); ++iter){
		Coin* coin = *iter;
		//不在屏幕
		if (coin->getPositionX() < -coin->getContentSize().width / 2){
			coin->setVisible(false);
		}
		//要是不可见，要不就是不在屏幕里，要不就是被碰撞了
		if (!coin->isVisible()){
			setNum++;
		}
		//让金币移动
		coin->setPositionX(coin->getPositionX() - 2);
	}
	if (setNum == 5){
		int i = 0;
		float posX = 640 + 50;
		float posY = 59 + 15 + CCRANDOM_0_1() * 160;
		for (Vector<Coin*>::iterator iter = this->m_coinArr.begin(); iter != this->m_coinArr.end(); ++iter){
			Coin* coin = *iter;
			//两个两个一起
			if (i < 2){
				posX += 100;//两个金币同一高度，间隔30
			}
			else{
				//重新重置位置 
				i = -1;
				posY = 49 + 15 + CCRANDOM_0_1() * 160;
				posX += 400;
			}
			i++;//重置金币个数记录
			//
			coin->setVisible(true);
			coin->setPosition(posX, posY);
		}
	}
	setNum = 0;
	//障碍物
	for (Vector<Barrier*>::iterator iter = this->m_barr.begin(); iter != this->m_barr.end(); ++iter){
		Barrier* barr = *iter;
		if (barr->getPositionX() < -barr->getContentSize().width / 2)
			barr->setVisible(false);
		if (!barr->isVisible())
			setNum++;
		//比背景移动的快一倍
		barr->setPositionX(barr->getPositionX() - 4);
	}
	if (setNum == 5){
		float posX = 640 + 50;
		float posY;
		for (Vector<Barrier*>::iterator iter = this->m_barr.begin(); iter != this->m_barr.end(); ++iter){
			auto barr = *iter;
			posX += 400;
			posY = 49 + 15 + CCRANDOM_0_1() * 160;
			barr->setPosition(posX, posY);
			barr->setVisible(true);
		}
	}
	
}