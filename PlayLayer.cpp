#include "PlayLayer.h"
#include "EndLayer.h"
#include "Popup.h"

//电脑上运行的时候设为True，点一下小人就可以跳跃，发布到手机端的时候设成False
#define onComputer false

PlayLayer::PlayLayer()
	:score(0),
	level(1),
	target(3000),
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
	auto scene = Scene::createWithPhysics();
	//最后的参数DEBUGDRAW_ALL影响的是显示的红框
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0,-600));
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

	//设置为单点响应
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
	if (!onComputer)
		touchListener->onTouchMoved = CC_CALLBACK_2(PlayLayer::onTouchMoved, this);// 触摸移动时触发
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayLayer::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//响应键盘消息
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(PlayLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

void PlayLayer::initBG(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//解决手机端的背景不显示的问题
	Point origin = Director::getInstance()->getVisibleOrigin();
	//初始化雾霾
	this->haze = Sprite::create("OverBG.png");
	//设置透明度
	haze->setOpacity(210);
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

	//分数背景
	auto board_score = Sprite::create("board_score.png");
	auto board_target = Sprite::create("board_target.png");
	board_score->setPosition(board_score->getContentSize().width/2,
							 visibleSize.height-board_score->getContentSize().height*2/3);
	board_target->setPosition(visibleSize.width-board_target->getContentSize().width/2,
								visibleSize.height-board_target->getContentSize().height*2/3);
	this->addChild(board_target,12);
	this->addChild(board_score,12);

	//分数和目标
	m_score = LabelTTF::create("distance", "fonts/JOKERMAN.TTF", 48);
	m_score->setPosition(board_score->getContentSize().width/2,
							visibleSize.height-board_score->getContentSize().height*2/3-10);
	m_score->setColor(Color3B(139, 248, 178));
	this->addChild(m_score,12);
	m_target = LabelTTF::create(CCString::createWithFormat("%d",target)->getCString(), "fonts/JOKERMAN.TTF", 48);
	m_target->setPosition(visibleSize.width-board_target->getContentSize().width/2,
							visibleSize.height-board_target->getContentSize().height*2/3-10);
	m_target->setColor(Color3B(255,209,27));
	this->addChild(m_target,12);
	//能量背景
	auto board_energy = Sprite::create("board_energy.png");
	board_energy->setPosition(visibleSize.width/2,visibleSize.height-board_energy->getContentSize().height*2/3);
	this->addChild(board_energy,12);
	//能量
	proBar = ProgressTimer::create(Sprite::create("energy.png"));
	proBar->setPosition(visibleSize.width/2+18,visibleSize.height-board_energy->getContentSize().height*2/3);
	proBar->setMidpoint(Point(0, 0));
	//从左向右递减
	proBar->setBarChangeRate(Point(1, 0));
	proBar->setType(ProgressTimer::Type::BAR);
	proBar->setPercentage(100);
	
	this->addChild(proBar,12);
}

//背景向后移动
void PlayLayer::update(float dt){
	int posX1 = bgSprite1->getPositionX();
	int posX2 = bgSprite2->getPositionX();
	float timer = proBar->getPercentage();

	//背景向后移动
	posX1 -= 2;
	posX2 -= 2;
	//距离增加
	this->score += 1;
	this->m_score->setString(CCString::createWithFormat("%d",score)->getCString());
	//能量（时间）条减少
	timer -= 0.05;
	proBar->setPercentage(timer);

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
	//检查是否游戏结束 移除屏幕外或者时间条到了
	if (hero->getPositionX()< 0 || timer <= 0){
		//Director::getInstance()->end();
		//跳转到失败界面
		Scene* newScene = EndLayer::createScene();
		EndLayer* layer = (EndLayer*)(newScene->getChildren().at(0));
		layer->setNext(false,0,0);
		layer->end_score->setString(CCString::createWithFormat("%d",score)->getCString());
		layer->target->setString(CCString::createWithFormat("%d",target)->getCString());
		layer->level->setString(CCString::createWithFormat("%d",level)->getCString());
		Director::sharedDirector()->replaceScene(newScene);
	}
	//围住小雨滴
	
	if (this->score >= this->target){
		Scene* newScene = GameLayer::createScene();
		GameLayer* layer = (GameLayer*)(newScene->getChildren().at(0));
		//设定下一关的初始值
		layer->setLevel(this->level);
		layer->setTarget(this->target);
		layer->setScore(this->score);
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
			this->proBar->setPercentage(proBar->getPercentage() + 10);
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

void PlayLayer::onKeyReleased(EventKeyboard::KeyCode keycode,Event* event){
	if(keycode == EventKeyboard::KeyCode::KEY_BACKSPACE){
		Director::sharedDirector()->pushScene(Popup::createScene());
	}
}

void PlayLayer::setLevel(unsigned lev){
	this->level = lev;
}

void PlayLayer::setTarget(unsigned tar){
	this->target = tar;
	this->m_target->setString(CCString::createWithFormat("%d",target*10)->getCString());
}

Sprite* PlayLayer::getHaze(){
	return this->haze;
}