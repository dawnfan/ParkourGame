#ifndef __COIN__H__
#define __COIN__H__

#include "cocos2d.h"
#include "Base.h"

USING_NS_CC;

class Coin : public Base{
public:
	virtual bool init();
	CREATE_FUNC(Coin);

};

#endif