#include "Base.h"

USING_NS_CC;

bool Base::init(){
	if (!Node::init()){
		return false;
	}
	return true;
}

Animate* Base::createAnimate(SpriteFrameCache* frameCache,int n,const char* name){
	SpriteFrame* frame = NULL;
	//数组不行 要用vector
	//auto frameArray = Array::create();
	//frameArray->retain();
	Vector<SpriteFrame*>frameArray;

	// 用一个列表保存所有SpriteFrame对象 
	for (int i = 0; i < n; i++) {
		/* 从SpriteFrame缓存池中获取CCSpriteFrame对象 */
		frame = frameCache->spriteFrameByName(String::createWithFormat("%s%d.png",name, i)->getCString());
		frameArray.pushBack(frame);

	}
	/* 使用SpriteFrame列表创建动画对象 */
	auto animation = Animation::createWithSpriteFrames(frameArray);

	animation->setLoops(-1);

	animation->setDelayPerUnit(0.1f);

	/* 将动画包装成一个动作 */
	auto action = Animate::create(animation);

	return action;
}

void Base::initBody(int mask){
	auto phyBody = PhysicsBody::createEdgeBox(sprite->getContentSize());
	phyBody->getShape(0)->setDensity(1000000);
	phyBody->getShape(0)->setMass(10000);
	phyBody->getShape(0)->setMaterial(PHYSICSBODY_MATERIAL_DEFAULT);
	phyBody->setCategoryBitmask(1);
	//设置为0，不会和奔跑的小人发生碰撞，因为是两个碰撞物体的掩码值逻辑与
	phyBody->setCollisionBitmask(mask);
	phyBody->setContactTestBitmask(1);

	this->setPhysicsBody(phyBody);
}

