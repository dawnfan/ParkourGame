#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "cocos2d.h"

USING_NS_CC;

class Background : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Background);

private:

};

#endif // __BACKGROUND_H__
