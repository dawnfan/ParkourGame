#ifndef __PLAYLAYER_H__
#define __PLAYLAYER_H__

#include "cocos2d.h"
#include "Runner.h"
#include "Manager.h"
#include "GameLayer.h"

#define BACKGROUND_SPEED 4
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
	bool onContactBegin(PhysicsContact& contact);
	//返回键响应
	void onKeyReleased(EventKeyboard::KeyCode keycode,Event* event);
	void setPhyWorld(PhysicsWorld* world){ m_world = world; }

	//用update函数让地图滚动
	virtual void update(float dt);

	void setLevel(unsigned lev);
	void setTarget(unsigned tar);
	ProgressTimer* getProbar();
	Sprite* getHaze();

private:
	//物理世界
	PhysicsWorld* m_world;
	//存储所有的帧图片
	Vector<SpriteFrame* >allframes;
	Animation* animation;
	Animate* animate;
	//奔跑的叶子
	Runner* hero;
	//背景精灵
	Sprite* bgSprite1;
	Sprite* bgSprite2;
	Sprite* groundSprite1;
	Sprite* groundSprite2;
	//用于管理金币等
	Manager* man;
	//触摸起始点
	int startPoint;
	//得分，决定是否过关
	unsigned score;
	//雾霾蒙版
	Sprite* haze;
	//当前关数
	unsigned level;
	//本关目标分数
	unsigned target;
	//显示：当前关数，目标分数，得分
	LabelTTF* m_level;
	LabelTTF* m_target;
	LabelTTF* m_score;
	//进度条
	ProgressTimer* proBar;


	//吃金币粒子特效
	void runEffect(Sprite* coin);
	//初始化背景
	void initBG();
};

#endif // __PLAYLAYER_H__
