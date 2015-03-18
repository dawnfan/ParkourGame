#ifndef __BaseManager__H__
#define __BaseManager__H__

#include "cocos2d.h"
#include "Base.h"

#define coinTag 2
#define barrTag 3
#define bugTag 4
#define WIDTH 1200
#define INTERVAL 100

USING_NS_CC;

class Manager : public cocos2d::Node{
public:
	CREATE_FUNC(Manager);
	virtual bool init();
	virtual void update(float dt);
	int height;

private:
	void createCoin();
	void createBarrier();
	void createBug();
	Vector<Base*> m_barr;
	Vector<Base*> m_coin;
	SpriteFrameCache* frameCache;
	Base* bug;

};/**/

#endif