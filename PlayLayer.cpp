#include "PlayLayer.h"

//电脑上运行的时候设为True，点一下小人就可以跳跃，发布到手机端的时候设成False
#define onComputer true

PlayLayer::PlayLayer()
	:counter(0),
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
	scene->getPhysicsWorld()->setGravity({ 0, -500 });
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

	scene->addChild(layer);
	return scene;
}

bool PlayLayer::init()
{
	if (!Layer::init()) {
		return false;
	}
	//start from here
	initBG();

	//开启update
	this->scheduleUpdate();
	hero = Runner::create();
	this->addChild(hero);
	hero->setPosition(300, 350);
	hero->Run();

	man = Manager::create();
	this->addChild(man);

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
	//背景1
	bgSprite1 = Sprite::create("Map00.png");
	bgSprite1->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bgSprite1);

	//地面1
	groundSprite1 = Sprite::create("Ground00.png");
	groundSprite1->setTag(11);
	groundSprite1->setPosition(visibleSize.width / 2, groundSprite1->getContentSize().height*2);
	auto body1 = PhysicsBody::createBox(groundSprite1->getContentSize());
	//弹性设为0，奔跑的东西不会颠倒
	body1->getShape(0)->setRestitution(0);
	body1->setDynamic(false);
	groundSprite1->setPhysicsBody(body1);
	this->addChild(groundSprite1);

	//背景2
	bgSprite2 = Sprite::create("Map01.png");
	bgSprite2->setPosition(bgSprite1->getContentSize().width + visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bgSprite2);

	//地面2
	groundSprite2 = Sprite::create("Ground01.png");
	groundSprite2->setTag(12);
	groundSprite2->setPosition(bgSprite1->getContentSize().width + visibleSize.width / 2,groundSprite2->getContentSize().height / 2);
	auto body2 = PhysicsBody::createBox(groundSprite2->getContentSize());
	body2->setDynamic(false);
	//弹性设为0，奔跑的东西不会颠倒
	body2->getShape(0)->setRestitution(0);
	groundSprite2->setPhysicsBody(body2);
	this->addChild(groundSprite2);
}

//背景向后移动
void PlayLayer::update(float dt){
	int posX1 = bgSprite1->getPositionX();
	int posX2 = bgSprite2->getPositionX();

	posX1 -= 2;
	posX2 -= 2;

	auto mapSize = bgSprite1->getContentSize();

	if (posX1 < -mapSize.width / 2){
		posX1 = mapSize.width + mapSize.width / 2;
		posX2 = mapSize.width / 2;
	}
	if (posX2 < -mapSize.width / 2){
		posX2 = mapSize.width + mapSize.width / 2;
		posX1 = mapSize.width / 2;
	}

	bgSprite1->setPositionX(posX1);
	bgSprite2->setPositionX(posX2);
	groundSprite1->setPositionX(posX1);
	groundSprite2->setPositionX(posX2);
	//检查是否游戏结束
	if (hero->getPositionX()< 0){
		Director::getInstance()->end();
	}
}

//触摸回调
bool PlayLayer::onTouchBegan(Touch *touch, Event *unused){
	if (onComputer)
		this->hero->Jump();
	else
		startPoint = touch->getLocation().x;
	return true;
}

void PlayLayer::onTouchMoved(Touch *touch, Event *unused){
	auto location = touch->getLocation();
	if (location.x > startPoint)
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