#ifndef __PLAYLAYER_H__
#define __PLAYLAYER_H__

#include "cocos2d.h"
#include "Runner.h"
#include "Manager.h"

USING_NS_CC;

class PlayLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(PlayLayer);

	PlayLayer();

	virtual bool onTouchBegan(Touch *touch, Event *unused) override;
	virtual void onTouchMoved(Touch *touch, Event *unused) override;
	void setPhyWorld(PhysicsWorld* world){ m_world = world; }
	//初始化背景
	void initBG();
	//用update函数让地图滚动
	virtual void update(float dt);
	void runEffect(Sprite* coin);
private:
	bool onContactBegin(PhysicsContact& contact);
	//物理世界
	PhysicsWorld* m_world;

	//存储所有的帧图片
	Vector<SpriteFrame* >allframes;
	Animation* animation;
	Animate* animate;
	//奔跑的叶子
	Runner* hero;
	//背景精灵
	cocos2d::Sprite* bgSprite1;
	cocos2d::Sprite* bgSprite2;
	cocos2d::Sprite* groundSprite1;
	cocos2d::Sprite* groundSprite2;
	//用于管理金币等
	Manager* man;
	//金币数量
	int counter;
	//触摸起始点
	int startPoint;
};

#endif // __PLAYLAYER_H__
