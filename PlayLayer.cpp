#include "PlayLayer.h"

//电脑上运行的时候设为True，点一下小人就可以跳跃，发布到手机端的时候设成False
#define onComputer true

PlayLayer::PlayLayer()
	:counter(0),
	score(0),
	level(1),
	target(1000),
	animation(NULL),
	animate(NULL),
	hero(NULL),
	bgSprite1(NULL),
	bgSprite2(NULL),
	groundSprite1(NULL),
	groundSprite2(NULL),
	man(NULL)
{

}

Scene *PlayLayer::createScene()
{
	//auto scene = Scene::create();
	//auto layer = PlayLayer::create();

	// add following codes
	auto scene = Scene::createWithPhysics();
	//最后的参数DEBUGDRAW_ALL影响的是显示的红框
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	//scene->getPhysicsWorld()->setGravity({0,-500});
	scene->getPhysicsWorld()->setGravity(Vect(0,-500));
	/*
	//创建一个边界
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//使用PhysicalsBody的create方法创建自己想要的物体。
	//PHYSICSBODY_MATERIAL_DEFAULT宏表示的是创建的Body的默认材质，3是边线宽度。编译运行我们会看到场景边上有红色的边界。

	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2, visibleSize.height /2));
	edgeNode->setPhysicsBody(body);
	scene->addChild(edgeNode);
	*/
	auto layer = PlayLayer::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	layer->setTag(13);
	scene->addChild(layer);
	return scene;
}

bool PlayLayer::init()
{
	if (!Layer::init()) {
		return false;
	}
	//start from here

	man = Manager::create();
	this->addChild(man, 3);

	initBG();

	//开启update
	this->scheduleUpdate();
	hero = Runner::create();
	this->addChild(hero,11);
	hero->setPosition(300, 350);
	hero->Run();



	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
	if (!onComputer)
		touchListener->onTouchMoved = CC_CALLBACK_2(PlayLayer::onTouchMoved, this);// 触摸移动时触发
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayLayer::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

void PlayLayer::initBG(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//解决手机端的背景不显示的问题
	Point origin = Director::getInstance()->getVisibleOrigin();
	//初始化雾霾
	this->haze = Sprite::create("OverBG.png");
	//设置透明度
	haze->setOpacity(0);
	haze->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(haze, 10);

	
	//背景1
	bgSprite1 = Sprite::create("bg1.png");
	bgSprite1->setPosition(origin.x + visibleSize.width / 2, origin.y+visibleSize.height / 2);
	this->addChild(bgSprite1,1);

	//地面1
	groundSprite1 = Sprite::create("ground.png");
	groundSprite1->setTag(11);
	groundSprite1->setPosition(origin.x + visibleSize.width / 2, origin.y + groundSprite1->getContentSize().height / 2);
	this->man->height = groundSprite1->getPositionY()+groundSprite1->getContentSize().height;
	auto body1 = PhysicsBody::createBox(groundSprite1->getContentSize());
	//弹性设为0，奔跑的东西不会颠倒
	//body1->getShape(0)->setRestitution(0);
	body1->setDynamic(false);
	groundSprite1->setPhysicsBody(body1);
	this->addChild(groundSprite1,2);


	//背景2
	bgSprite2 = Sprite::create("bg2.png");
	bgSprite2->setPosition(bgSprite1->getContentSize().width + visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bgSprite2,1);

	//地面2
	groundSprite2 = Sprite::create("ground.png");
	groundSprite2->setTag(12);
	groundSprite2->setPosition(bgSprite1->getContentSize().width + visibleSize.width / 2, groundSprite2->getContentSize().height / 2);
	auto body2 = PhysicsBody::createBox(groundSprite2->getContentSize());
	body2->setDynamic(false);
	//弹性设为0，奔跑的东西不会颠倒
	//body2->getShape(0)->setRestitution(0);
	groundSprite2->setPhysicsBody(body2);
	this->addChild(groundSprite2,2);

}

//背景向后移动
void PlayLayer::update(float dt){
	int posX1 = bgSprite1->getPositionX();
	int posX2 = bgSprite2->getPositionX();

	posX1 -= 2;
	posX2 -= 2;
	this->score += 1;

	auto mapSize = bgSprite1->getContentSize();

	if (posX1 < -mapSize.width / 2){
		posX1 = mapSize.width + mapSize.width / 2;
		posX2 = mapSize.width / 2;
		this->man->height = groundSprite2->getPositionY()+groundSprite2->getContentSize().height;
	}
	if (posX2 < -mapSize.width / 2){
		posX2 = mapSize.width + mapSize.width / 2;
		posX1 = mapSize.width / 2;
		this->man->height = groundSprite1->getPositionY()+groundSprite1->getContentSize().height;
	}

	bgSprite1->setPositionX(posX1);
	bgSprite2->setPositionX(posX2);
	groundSprite1->setPositionX(posX1);
	groundSprite2->setPositionX(posX2);
	//检查是否游戏结束
	if (hero->getPositionX()< 0){
		Director::getInstance()->end();
	}
	//围住小雨滴
	
	if (this->score >= 10*this->target){
		Scene* newScene = GameLayer::createScene();
		GameLayer* layer = (GameLayer*)(newScene->getChildren().at(0));
		//设定下一关的初始值
		layer->setLevel(this->level);
		layer->setTarget(this->target);
		Director::sharedDirector()->replaceScene(newScene);
	}
}

//触摸回调
bool PlayLayer::onTouchBegan(Touch *touch, Event *unused){
	if (onComputer)
		this->hero->Jump();
	else
		startPoint = touch->getLocation().y;
	return true;
}

void PlayLayer::onTouchMoved(Touch *touch, Event *unused){
	auto location = touch->getLocation();
	if (location.y > startPoint)
		this->hero->Jump();
}

//碰撞回调
bool PlayLayer::onContactBegin(PhysicsContact& contact)
{
	auto body_1 = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto body_2 = (Sprite*)contact.getShapeB()->getBody()->getNode();
	//碰到金币
	if (body_1->getTag() == 2 && body_2->getTag() == 1){
		if (body_1->isVisible()){
			runEffect(body_2);
			this->counter++;
		}
		body_1->setVisible(false);
	}
	return true;
}

void PlayLayer::runEffect(Sprite* coin){
	double time = 0.2;
	//1.action for circle
	
	auto circleSprite = Sprite::create("circle.png");
	this->addChild(circleSprite, 10);
	circleSprite->setPosition(coin->getPosition());
	circleSprite->setScale(0);// start size
	circleSprite->runAction(Sequence::create(ScaleTo::create(time, 2.0),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circleSprite)),
		NULL));
	
	//2.particle effect
	auto particleStars = ParticleSystemQuad::create("stars.plist");
	particleStars->setAutoRemoveOnFinish(true);
	particleStars->setBlendAdditive(false);
	particleStars->setPosition(coin->getPosition());
	particleStars->setScale(0.3);
	this->addChild(particleStars, 20);
}

void PlayLayer::setLevel(unsigned lev){
	this->level = lev;
}

void PlayLayer::setTarget(unsigned tar){
	this->target = tar;
}

Sprite* PlayLayer::getHaze(){
	return this->haze;
}