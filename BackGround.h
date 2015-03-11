#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "cocos2d.h"

USING_NS_CC;

class Background : public cocos2d::Node{
public:
	virtual bool init();
	CREATE_FUNC(Background);
private:
	Sprite* bgSprite;
	//Sprite* groundSprite;
	Size visibleSize;
};

#endif // __BACKGROUND_H__
