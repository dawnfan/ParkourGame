#include "GameLayer.h"

#define MATRIX_WIDTH (9)
#define MATRIX_HEIGHT (9)

#define SPIRTE_GAP (1)

#define userDefault UserDefault::getInstance()

int choices[2][6][2] = {
	{
		{1,-1},{1,0},{0,1},{-1,0},{-1,-1},{0,-1}
	},
	{
		{1,0},{1,1},{0,1},{-1,1},{-1,0},{0,-1}
	}
};

GameLayer::GameLayer()
	:m_exit(NULL),
	m_matrix(NULL),
	m_mark(NULL),
	//spriteSheet(NULL),
	m_width(0),
	m_height(0),
	m_isTouchEnable(true),
	m_raindrop(NULL),
	m_markSize(0),
	m_isRun(false),
	m_times(0)
{

}

GameLayer::~GameLayer()
{
	if(m_matrix){
		free(m_matrix);
	}
	if(m_mark){
		free(m_mark);
	}
}

Scene *GameLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = GameLayer::create();
	scene->addChild(layer);
	return scene;
}

bool GameLayer::init()
{
	if (!Layer::init()) {
		return false;
	}

	// background
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create("main_game/lawn.png");
	background->setAnchorPoint(Point(0, 1));
	background->setPosition(Point(0, winSize.height));
	this->addChild(background);

	m_height = MATRIX_HEIGHT;
	m_width = MATRIX_WIDTH;
	//初始化地图矩阵
	int arraySize = sizeof(SquareSprite *) * m_width * m_height;
	m_matrix = (SquareSprite **)malloc(arraySize);
	memset((void*)m_matrix, 0, arraySize);
	//初始化标记数组
	m_markSize = sizeof(bool) * m_width * m_height;
	m_mark = (bool*)malloc(m_markSize);
	memset((void*)m_mark,false,m_markSize);
	//初始化所有地图精灵
	for(int row = 0;row<m_height;++row)
	{
		for(int col = 0;col<m_width;++col)
		{
			SquareSprite * sprite = SquareSprite::create(row,col);
			Point position = positionOfItem(row, col);
			sprite->setPosition(position);
			addChild(sprite,1);
			m_matrix[row * m_width + col] = sprite;
		}
	}
	//随机初始化云彩
	initCloud();
	//初始化小雨滴
	m_raindrop = Raindrop::create(4,4);
	Point position = positionOfItem(4, 4);
	m_raindrop->setPosition(position);
	addChild(m_raindrop,2);
	
	//设置为单点响应
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	// bind touch event实现触摸效果
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//响应键盘消息
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}
//得到坐标
Point GameLayer::positionOfItem(int row, int col)
{
	float x = (SquareSprite::getContentWidth() + SPIRTE_GAP) * col + SquareSprite::getContentWidth() / 2;
	float y = (SquareSprite::getContentWidth() + SPIRTE_GAP) * row + SquareSprite::getContentWidth() / 2 + 45;
	if(row%2)//形成偏移的效果
	{
		x = x+SquareSprite::getContentWidth() / 2;
	}
	return Point(x, y);
}
//响应安卓返回键
void GameLayer::onKeyReleased(EventKeyboard::KeyCode keycode,Event* event)
{
	if(keycode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{//转入暂停页面
		CCDirector::sharedDirector()->pushScene(Popup::createScene());
	}
}
//获取点击事件
bool GameLayer::onTouchBegan(Touch *touch, Event *unused)
{
	if (m_isTouchEnable ) 
	{
		//点击出现云彩
		auto location = touch->getLocation();
		SquareSprite* srcSprite = spriteOfPoint(&location);
		if(srcSprite)
		{
			if(!srcSprite->getSelected())//未被选择过
			{
				m_times++;
				srcSprite->setSelected(true);
				CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage("block2.png");
				srcSprite->setTexture(texture);
				moveRaindrop();
			}
		}
	}
	return m_isTouchEnable;
}
//得到点击了哪个格子
SquareSprite *GameLayer::spriteOfPoint(Point *point)
{
	SquareSprite *sprite = NULL;
	Rect rect = Rect(0, 0, 0, 0);

	for (int i = 0; i < m_height * m_width; i++) {
		sprite = m_matrix[i];
		if (sprite) {
			rect.origin.x = sprite->getPositionX() - (sprite->getContentSize().width / 2);
			rect.origin.y = sprite->getPositionY() - (sprite->getContentSize().height / 2);
			rect.size = sprite->getContentSize();
			//修改，空隙也属于精灵的一部分
			rect.size.height += SPIRTE_GAP;
			rect.size.width += SPIRTE_GAP;
			if (rect.containsPoint(*point)) {
				return sprite;
			}
		}
	}
	return NULL;
}
//随机初始化云彩
void GameLayer::initCloud()
{
	srand(time(NULL));
	//初始化云彩的数量
	int num = rand()%14 + 7;
	//初始化云彩的位置
	while(num--)
	{
		int col = rand()%9;
		int row = rand()%9;
		if(col != 4 && row != 4)
		{
			SquareSprite* srcSprite = m_matrix[row*MATRIX_WIDTH + col];
			srcSprite->setSelected(true);
			CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage("block2.png");
			srcSprite->setTexture(texture);
		}
	}
}
//移动雨滴
void GameLayer::moveRaindrop()
{
	int row = m_raindrop->getRow();
	int col = m_raindrop->getCol();
	int odd = row%2;
	//每次调用都重新初始化标记数组
	memset((void*)m_mark,false,m_markSize);
	//当前坐标也标记为走过了
	m_mark[row*m_width+col] = true;
	SquareSprite* now_sprite = m_matrix[row*MATRIX_WIDTH + col];
	SquareSprite* end_sprite = findNext(now_sprite);
	if(end_sprite){
		SquareSprite* next = end_sprite->getPrev();
		if(next == now_sprite){
			m_isRun = true;
		}
		while(next != now_sprite){
			end_sprite = next;
			next = next->getPrev();
		}
		next = end_sprite;
		m_raindrop->runAction(MoveTo::create(0.3, next->getPosition()));
		m_raindrop->setCol(next->getCol());
		m_raindrop->setRow(next->getRow());
		if(m_isRun){
			//雨滴逃脱
			//转换到胜利页面，指示没有抓住
			Scene* newScene = EndLayer::createScene();
			EndLayer* layer = (EndLayer*)(newScene->getChildren().at(0));
			layer->setNext(true,this->level + 1,this->target + 500);
			layer->prompt->setString("YOU DIDN'T CATCH THE DROP !");
			layer->end_score->setString(CCString::createWithFormat("%d",this->score)->getCString());
			layer->level->setString(CCString::createWithFormat("%d",this->level)->getCString());
			layer->target->setString(CCString::createWithFormat("%d",this->target)->getCString());
			Director::sharedDirector()->replaceScene(newScene);
		}
	}else{
		//雨滴被围住了,随便找一个方向走
		SquareSprite* next = NULL;
		for(int i = 0;i<6;++i)
		{
			int next_row = row + choices[odd][i][0];
			int next_col = col + choices[odd][i][1];
			SquareSprite* next_sprite = m_matrix[next_row*MATRIX_WIDTH + next_col];
			if(!next_sprite->getSelected())//可以走就走
			{
				next = next_sprite;
			}
		}
		if(next){
			m_raindrop->runAction(MoveTo::create(0.3, next->getPosition()));
			m_raindrop->setCol(next->getCol());
			m_raindrop->setRow(next->getRow());
		}
		else{
			//转换到胜利页面，指示没有抓住
			Scene* newScene = EndLayer::createScene();
			EndLayer* layer = (EndLayer*)(newScene->getChildren().at(0));
			layer->setCatchedDrop();
			layer->setNext(true,this->level + 1,this->target + 500);
			layer->prompt->setString("YOU GET EXTRA ENERGY !");
			layer->end_score->setString(CCString::createWithFormat("%d",this->score)->getCString());
			layer->level->setString(CCString::createWithFormat("%d",this->level)->getCString());
			layer->target->setString(CCString::createWithFormat("%d",this->target)->getCString());
			Director::sharedDirector()->replaceScene(newScene);
		}
	}

}
//另一种算法，遍历，类似于二叉树层次遍历
SquareSprite* GameLayer::findNext(SquareSprite* pos)
{
	std::queue<SquareSprite*> myQueue;
	//push 之前把方块设为已经在队列里面
	//pos->setQue(true);
	myQueue.push(pos);
	while(!myQueue.empty()){
		SquareSprite* now = myQueue.front();
		myQueue.pop();
		//POP之后把方块设为不在队列里面
		//now->setQue(false);
		int row = now->getRow();
		int col = now->getCol();
		int odd = row%2;
		for(int i = 0;i<6;++i)//查找能走的六个方向
		{
			int next_row = row + choices[odd][i][0];
			int next_col = col + choices[odd][i][1];
			SquareSprite* next_sprite = m_matrix[next_row*MATRIX_WIDTH + next_col];
			if(!next_sprite->getSelected() && m_mark[next_row*m_width+next_col] == false)//没变成云
			{
				next_sprite->setPrev(now);
				if(next_col <= 0 || next_col >= 8 || next_row <= 0 || next_row >= 8){//是边界的话直接返回
					return next_sprite;
				}else{
					//next_sprite->setQue(true);
					m_mark[next_row*m_width+next_col] = true;
					myQueue.push(next_sprite);
				}
			}
		}
	}
	//返回空值的时候说明雨滴已经被围住了
	return NULL;
}

void GameLayer::setLevel(unsigned lev){
	this->level = lev;
}

void GameLayer::setTarget(unsigned tar){
	this->target = tar;
}

void GameLayer::setScore(unsigned sco){
	this->score = sco;
}