#include "Runner.h"

USING_NS_CC;

bool Runner::init(){
	auto frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("hazefight.plist", "hazefight.png");
	m_runner = Sprite::createWithSpriteFrameName("run0.png");

	run_jumpSize = m_runner->getContentSize();
	crouchSize = m_runner->getContentSize();

	this->addChild(m_runner);

	initActionSet();//初始化动作集合

	initBody();

	this->scheduleUpdate();

	this->setTag(1);

	return true;
}

void Runner::initActionSet(){
	auto frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("hazefight.plist", "hazefight.png");
	SpriteFrame* frame = NULL;
	//3.0中改用vector 而不是用Array
	Vector<SpriteFrame*>frameVector;

	/* 1.----------------加载跑动的Animation-----------------*/
	for (int i = 0; i < 6; i++) {
		//从缓存池中加载精灵到Vector
		//frame = SpriteFrame::create(StringUtils::format("ani_blow/runner%d.png", i), Rect(0, 0, 500, 500));
		frame = frameCache->spriteFrameByName(String::createWithFormat("run%d.png", i)->getCString());
		frameVector.pushBack(frame);
	}

	//用vector里面的SpriteFrame列表创建Animation  以及设置一些参数
	auto run_animation = Animation::createWithSpriteFrames(frameVector, 0.1f, -1);
	//将跑动的 Animation 取名为 running 
	AnimationCache::getInstance()->addAnimation(run_animation, "running");

	/*4---------------*/
	frameVector.clear();
	for (int i = 0; i <= 3; i++){
		//frame = SpriteFrame::create(StringUtils::format("ani_roll/runnerJumpUp%d.png", i), Rect(0, 0, 500, 500));
		frame = frameCache->spriteFrameByName(String::createWithFormat("up%d.png", i)->getCString());
		frameVector.pushBack(frame);
	}
	auto jumpUp_animation = Animation::createWithSpriteFrames(frameVector, 0.2);//不设置无限循环
	AnimationCache::getInstance()->addAnimation(jumpUp_animation, "jumpUp");
	
	
	frameVector.clear();
	for (int i = 0; i <= 1; i++){
		//frame = SpriteFrame::create(StringUtils::format("ani_roll/runnerJumpDown%d.png", i),Rect(0,0,500,500));
		frame = frameCache->spriteFrameByName(String::createWithFormat("down%d.png", i)->getCString());
		frameVector.pushBack(frame);
	}
	auto jumpDown_animation = Animation::createWithSpriteFrames(frameVector, 0.3);
	AnimationCache::getInstance()->addAnimation(jumpDown_animation, "jumpDown");
	
}

void Runner::doAction(const char* actionName){
	auto animation = AnimationCache::getInstance()->animationByName(actionName);
	auto action = Animate::create(animation);
	m_runner->runAction(action);
}

void Runner::initBody(){
	//根据不同状态设置不同刚体大小
	Size bodySize;
	if (m_state == crouch){
		bodySize = crouchSize;
	}
	else{
		bodySize = run_jumpSize;
	}

	//创建runner的刚体
	auto runerBody = PhysicsBody::createBox(bodySize, PHYSICSBODY_MATERIAL_DEFAULT);
	//解决小人翻倒的问题
	runerBody->setRotationEnable(false);
	runerBody->getShape(0)->setRestitution(0);
	//设置可以碰撞检测
	runerBody->setCategoryBitmask(1);
	runerBody->setCollisionBitmask(1);
	runerBody->setContactTestBitmask(1);

	//绑定刚体
	this->setPhysicsBody(runerBody);
}

void Runner::Run(){
	m_state = running;
	initBody();
	doAction("running");
}

/**/
void Runner::Jump(){
	//只有在跑动时才能起跳
	if (m_state == running){
		m_state = jumpUp;

		auto mass = this->getPhysicsBody()->getMass() * 350;

		this->getPhysicsBody()->applyImpulse(Vect(0, mass));

		m_runner->stopAllActions();

		doAction("jumpUp");
	}
}

void Runner::update(float dt){
	auto vel = this->getPhysicsBody()->getVelocity();
	if (m_state == jumpUp){
		if (vel.y < 0.1){
			m_state = jumpDown;
			m_runner->stopAllActions();
			doAction("jumpDown");
		}
	}
	if (m_state == jumpDown){
		CCLOG("%f", vel.y);
		//不应该是 等于 0
		if (vel.y > 0){
			m_state = running;
			m_runner->stopAllActions();
			doAction("running");
		}
	}
}

void Runner::Crouch(){
	//只能在跑动的时候蹲下
	if (m_state == running){
		m_state = crouch;
		m_runner->stopAllActions();
		initBody();
		doAction("crouch");
	}
}