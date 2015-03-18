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
	//��ʼ����ͼ����
	int arraySize = sizeof(SquareSprite *) * m_width * m_height;
	m_matrix = (SquareSprite **)malloc(arraySize);
	memset((void*)m_matrix, 0, arraySize);
	//��ʼ���������
	m_markSize = sizeof(bool) * m_width * m_height;
	m_mark = (bool*)malloc(m_markSize);
	memset((void*)m_mark,false,m_markSize);
	//��ʼ�����е�ͼ����
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
	//�����ʼ���Ʋ�
	initCloud();
	//��ʼ��С���
	m_raindrop = Raindrop::create(4,4);
	Point position = positionOfItem(4, 4);
	m_raindrop->setPosition(position);
	addChild(m_raindrop,2);
	
	//����Ϊ������Ӧ
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	// bind touch eventʵ�ִ���Ч��
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//��Ӧ������Ϣ
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}
//�õ�����
Point GameLayer::positionOfItem(int row, int col)
{
	float x = (SquareSprite::getContentWidth() + SPIRTE_GAP) * col + SquareSprite::getContentWidth() / 2;
	float y = (SquareSprite::getContentWidth() + SPIRTE_GAP) * row + SquareSprite::getContentWidth() / 2 + 45;
	if(row%2)//�γ�ƫ�Ƶ�Ч��
	{
		x = x+SquareSprite::getContentWidth() / 2;
	}
	return Point(x, y);
}
//��Ӧ��׿���ؼ�
void GameLayer::onKeyReleased(EventKeyboard::KeyCode keycode,Event* event)
{
	if(keycode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{//ת����ͣҳ��
		CCDirector::sharedDirector()->pushScene(Popup::createScene());
	}
}
//��ȡ����¼�
bool GameLayer::onTouchBegan(Touch *touch, Event *unused)
{
	if (m_isTouchEnable ) 
	{
		//��������Ʋ�
		auto location = touch->getLocation();
		SquareSprite* srcSprite = spriteOfPoint(&location);
		if(srcSprite)
		{
			if(!srcSprite->getSelected())//δ��ѡ���
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
//�õ�������ĸ�����
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
			//�޸ģ���϶Ҳ���ھ����һ����
			rect.size.height += SPIRTE_GAP;
			rect.size.width += SPIRTE_GAP;
			if (rect.containsPoint(*point)) {
				return sprite;
			}
		}
	}
	return NULL;
}
//�����ʼ���Ʋ�
void GameLayer::initCloud()
{
	srand(time(NULL));
	//��ʼ���Ʋʵ�����
	int num = rand()%14 + 7;
	//��ʼ���Ʋʵ�λ��
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
//�ƶ����
void GameLayer::moveRaindrop()
{
	int row = m_raindrop->getRow();
	int col = m_raindrop->getCol();
	int odd = row%2;
	//ÿ�ε��ö����³�ʼ���������
	memset((void*)m_mark,false,m_markSize);
	//��ǰ����Ҳ���Ϊ�߹���
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
			//�������
			//ת����ʤ��ҳ�棬ָʾû��ץס
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
		//��α�Χס��,�����һ��������
		SquareSprite* next = NULL;
		for(int i = 0;i<6;++i)
		{
			int next_row = row + choices[odd][i][0];
			int next_col = col + choices[odd][i][1];
			SquareSprite* next_sprite = m_matrix[next_row*MATRIX_WIDTH + next_col];
			if(!next_sprite->getSelected())//�����߾���
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
			//ת����ʤ��ҳ�棬ָʾû��ץס
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
//��һ���㷨�������������ڶ�������α���
SquareSprite* GameLayer::findNext(SquareSprite* pos)
{
	std::queue<SquareSprite*> myQueue;
	//push ֮ǰ�ѷ�����Ϊ�Ѿ��ڶ�������
	//pos->setQue(true);
	myQueue.push(pos);
	while(!myQueue.empty()){
		SquareSprite* now = myQueue.front();
		myQueue.pop();
		//POP֮��ѷ�����Ϊ���ڶ�������
		//now->setQue(false);
		int row = now->getRow();
		int col = now->getCol();
		int odd = row%2;
		for(int i = 0;i<6;++i)//�������ߵ���������
		{
			int next_row = row + choices[odd][i][0];
			int next_col = col + choices[odd][i][1];
			SquareSprite* next_sprite = m_matrix[next_row*MATRIX_WIDTH + next_col];
			if(!next_sprite->getSelected() && m_mark[next_row*m_width+next_col] == false)//û�����
			{
				next_sprite->setPrev(now);
				if(next_col <= 0 || next_col >= 8 || next_row <= 0 || next_row >= 8){//�Ǳ߽�Ļ�ֱ�ӷ���
					return next_sprite;
				}else{
					//next_sprite->setQue(true);
					m_mark[next_row*m_width+next_col] = true;
					myQueue.push(next_sprite);
				}
			}
		}
	}
	//���ؿ�ֵ��ʱ��˵������Ѿ���Χס��
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