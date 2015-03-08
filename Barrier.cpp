#include "Barrier.h"

USING_NS_CC;

bool Barrier::init(){
	if (!Node::init()){
		return false;
	}

	//缓存池
	auto frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("parkour.plist", "parkour.png");

	sprite = Sprite::createWithSpriteFrameName("rock.png");

	//绑定
	//setSprite(sprite);
	this->addChild(sprite);
	//执行动作
	//sprite->runAction(createAnimate(frameCache));
	//绑定刚体
	initBody(1);

	return true;
}
