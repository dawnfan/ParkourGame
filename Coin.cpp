#include "Coin.h"

USING_NS_CC;

bool Coin::init(){
	if (!Node::init()){
		return false;
	}

	//缓存池
	auto frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("parkour.plist", "parkour.png");

	sprite = Sprite::createWithSpriteFrameName("coin0.png");

	//绑定
	//setSprite(sprite);
	this->addChild(sprite);
	//执行动作
	sprite->runAction(createAnimate(frameCache));
	//绑定刚体
	initBody(0);

	return true;
}
