#ifndef __BaseManager__H__
#define __BaseManager__H__

#include "cocos2d.h"
#include "Coin.h"
#include "Barrier.h"

#define coinTag 2
#define barrTag 3

USING_NS_CC;

class Manager : public cocos2d::Node{
public:
	CREATE_FUNC(Manager);
	virtual bool init();
	virtual void update(float dt);

private:
	void createCoin();
	void createBarrier();
	Vector<Barrier*> m_barr;
	Vector<Coin*> m_coinArr;

};/**/

#endif