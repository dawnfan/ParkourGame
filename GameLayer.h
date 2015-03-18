#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include "SquareSprite.h"
#include "Raindrop.h"
#include "PlayLayer.h"
#include "Popup.h"
#include "EndLayer.h"


USING_NS_CC;

class GameLayer : public Layer
{
public:
	GameLayer();
	~GameLayer();
	static Scene* createScene();
	CREATE_FUNC(GameLayer);

	virtual bool init() override;
	virtual bool onTouchBegan(Touch *touch, Event *unused) override;
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode,Event* event)override;

	void ReturnToMainFunc(cocos2d::Ref* pSender);
	void setLevel(unsigned lev);
	void setTarget(unsigned tar);
	void setScore(unsigned sco);
private:
	//���ؼ�����
	LabelTTF * m_exit;
	//����ָ�����з������ָ��
	SquareSprite ** m_matrix;
	//��ǣ�һ��ѡ·�������Ƿ��ҹ��þ���
	bool * m_mark;
	//SpriteBatchNode *spriteSheet;
	int m_width;
	int m_height;
	bool m_isTouchEnable;
	Raindrop* m_raindrop;
	int m_markSize;
	bool m_isRun;
	int m_times;
	//��ǰ����
	unsigned level;
	//����Ŀ�����
	unsigned target;
	//��ǰ���÷���
	unsigned score;

	Point positionOfItem(int row, int col);
	int findWay(int row,int col,int result,int tag);
	SquareSprite* findNext(SquareSprite* pos);
	SquareSprite* spriteOfPoint(Point *point);
	void initCloud();
	void moveRaindrop();

};
#endif /* defined(__GAMELAYER_H__) */