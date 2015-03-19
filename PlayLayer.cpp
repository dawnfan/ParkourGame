#include "PlayLayer.h"
#include "EndLayer.h"
#include "Popup.h"

//���������е�ʱ����ΪTrue����һ��С�˾Ϳ�����Ծ���������ֻ��˵�ʱ�����False
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
	//���Ĳ���DEBUGDRAW_ALLӰ�������ʾ�ĺ��
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0,-600));
	/*
	//����һ���߽�
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//ʹ��PhysicalsBody��create���������Լ���Ҫ�����塣
	//PHYSICSBODY_MATERIAL_DEFAULT���ʾ���Ǵ�����Body��Ĭ�ϲ��ʣ�3�Ǳ��߿�ȡ������������ǻῴ�����������к�ɫ�ı߽硣

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

	//����update
	this->scheduleUpdate();
	hero = Runner::create();
	this->addChild(hero,11);
	hero->setPosition(300, 350);
	hero->Run();

	//����Ϊ������Ӧ
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
	if (!onComputer)
		touchListener->onTouchMoved = CC_CALLBACK_2(PlayLayer::onTouchMoved, this);// �����ƶ�ʱ����
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayLayer::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//��Ӧ������Ϣ
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(PlayLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

void PlayLayer::initBG(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//����ֻ��˵ı�������ʾ������
	Point origin = Director::getInstance()->getVisibleOrigin();
	//��ʼ������
	this->haze = Sprite::create("OverBG.png");
	//����͸����
	haze->setOpacity(210);
	haze->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(haze, 10);
	//���ݵ�ǰ�ؿ�ѡ��ͬ�ı���
	int bgid = level % 4 - 1;
	//����1
	bgSprite1 = Sprite::create(CCString::createWithFormat("bg/bg%d.png", bgid)->getCString());
	bgSprite1->setPosition(origin.x + visibleSize.width / 2, origin.y+visibleSize.height / 2);
	this->addChild(bgSprite1,1);

	//����1
	groundSprite1 = Sprite::create("ground.png");
	groundSprite1->setTag(11);
	groundSprite1->setPosition(origin.x + visibleSize.width / 2, origin.y + groundSprite1->getContentSize().height / 2);
	this->man->height = groundSprite1->getPositionY()+groundSprite1->getContentSize().height;
	auto body1 = PhysicsBody::createBox(groundSprite1->getContentSize());
	//������Ϊ0�����ܵĶ�������ߵ�
	//body1->getShape(0)->setRestitution(0);
	body1->setDynamic(false);
	groundSprite1->setPhysicsBody(body1);
	this->addChild(groundSprite1,2);


	//����2
	bgSprite2 = Sprite::create(CCString::createWithFormat("bg/bg%d.png", bgid+1)->getCString());
	bgSprite2->setPosition(bgSprite1->getContentSize().width + visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bgSprite2,1);

	//����2
	groundSprite2 = Sprite::create("ground.png");
	groundSprite2->setTag(12);
	groundSprite2->setPosition(bgSprite1->getContentSize().width + visibleSize.width / 2, groundSprite2->getContentSize().height / 2);
	auto body2 = PhysicsBody::createBox(groundSprite2->getContentSize());
	body2->setDynamic(false);
	//������Ϊ0�����ܵĶ�������ߵ�
	//body2->getShape(0)->setRestitution(0);
	groundSprite2->setPhysicsBody(body2);
	this->addChild(groundSprite2,2);

	//�ؿ�
	m_level = LabelTTF::create("Level 1","fonts/jokerman.ttf", 48);
	m_level->setPosition(visibleSize.width/2,visibleSize.height-30);
	m_level->setColor(Color3B::YELLOW);
	this->addChild(m_level,12);

	//��������
	auto board_score = Sprite::create("board_score.png");
	auto board_target = Sprite::create("board_target.png");
	board_score->setPosition(board_score->getContentSize().width/2,
							 visibleSize.height-board_score->getContentSize().height*2/3);
	board_target->setPosition(visibleSize.width-board_target->getContentSize().width/2,
								visibleSize.height-board_target->getContentSize().height*2/3);
	this->addChild(board_target,13);
	this->addChild(board_score,13);

	//������Ŀ��
	m_score = LabelTTF::create("distance", "fonts/jokerman.ttf", 48);
	m_score->setPosition(board_score->getContentSize().width/2,
							visibleSize.height-board_score->getContentSize().height*2/3-10);
	m_score->setColor(Color3B(139, 248, 178));
	this->addChild(m_score,14);
	m_target = LabelTTF::create(CCString::createWithFormat("%d",target)->getCString(), "fonts/jokerman.ttf", 48);
	m_target->setPosition(visibleSize.width-board_target->getContentSize().width/2,
							visibleSize.height-board_target->getContentSize().height*2/3-10);
	m_target->setColor(Color3B(255,209,27));
	this->addChild(m_target,14);
	//��������
	auto board_energy = Sprite::create("board_energy.png");
	board_energy->setPosition(visibleSize.width/2,visibleSize.height-board_energy->getContentSize().height*3/2+10);
	this->addChild(board_energy,12);
	//����
	proBar = ProgressTimer::create(Sprite::create("energy.png"));
	proBar->setPosition(visibleSize.width/2+18,visibleSize.height-board_energy->getContentSize().height*3/2+10);
	proBar->setMidpoint(Point(0, 0));
	//�������ҵݼ�
	proBar->setBarChangeRate(Point(1, 0));
	proBar->setType(ProgressTimer::Type::BAR);
	proBar->setPercentage(60);
	
	this->addChild(proBar,15);
}

//��������ƶ�
void PlayLayer::update(float dt){
	int posX1 = bgSprite1->getPositionX();
	int posX2 = bgSprite2->getPositionX();
	float timer = proBar->getPercentage();

	//��������ƶ�
	posX1 -= BACKGROUND_SPEED;
	posX2 -= BACKGROUND_SPEED;
	//��������
	this->score += 1;
	this->m_score->setString(CCString::createWithFormat("%d",score)->getCString());
	//������ʱ�䣩������
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
	//����Ƿ���Ϸ���� �Ƴ���Ļ�����ʱ��������
	if (hero->getPositionX()< 0 || timer <= 0){
		//Director::getInstance()->end();
		//��ת��ʧ�ܽ���
		Scene* newScene = EndLayer::createScene();
		EndLayer* layer = (EndLayer*)(newScene->getChildren().at(0));
		layer->setNext(false,0,0);
		layer->end_score->setString(CCString::createWithFormat("%d",score)->getCString());
		layer->target->setString(CCString::createWithFormat("%d",target)->getCString());
		layer->level->setString(CCString::createWithFormat("%d",level)->getCString());
		Director::sharedDirector()->replaceScene(newScene);
	}
	//ΧסС���
	
	if (this->score >= this->target){
		Scene* newScene = GameLayer::createScene();
		GameLayer* layer = (GameLayer*)(newScene->getChildren().at(0));
		//�趨��һ�صĳ�ʼֵ
		layer->setLevel(this->level);
		layer->setTarget(this->target);
		layer->setScore(this->score);
		Director::sharedDirector()->replaceScene(newScene);
	}
}

//�����ص�
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

//��ײ�ص�
bool PlayLayer::onContactBegin(PhysicsContact& contact)
{
	auto body_1 = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto body_2 = (Sprite*)contact.getShapeB()->getBody()->getNode();
	//�������
	if (body_1->getTag() == 2 && body_2->getTag() == 1){
		if (body_1->isVisible()){
			runEffect(body_2);
			this->proBar->setPercentage(proBar->getPercentage() + 10);
		}
		body_1->setVisible(false);
	}
	//��������
	else if (body_1->getTag() == 4 && body_2->getTag() == 1){
		if (body_1->isVisible()){
			body_2->runAction(Blink::create(0.5,3));
			this->proBar->setPercentage(proBar->getPercentage() - 10);
		}
		body_1->setVisible(false);
	}
	else if (body_1->getTag() == 1 && body_2->getTag() == 4){
		if (body_2->isVisible()){
			body_1->runAction(Blink::create(0.5, 3));
			this->proBar->setPercentage(proBar->getPercentage() - 10);
		}
		body_2->setVisible(false);
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
	int bgid = (lev-1) % 4;
	this->m_level->setString(CCString::createWithFormat("Level %d",this->level)->getCString());
	this->bgSprite1->setTexture(CCString::createWithFormat("bg/bg%d.png", bgid)->getCString());
	this->bgSprite2->setTexture(CCString::createWithFormat("bg/bg%d.png", bgid+1)->getCString());
}

void PlayLayer::setTarget(unsigned tar){
	this->target = tar;
	this->m_target->setString(CCString::createWithFormat("%d",target)->getCString());
}

Sprite* PlayLayer::getHaze(){
	return this->haze;
}

ProgressTimer* PlayLayer::getProbar(){
	return this->proBar;
}