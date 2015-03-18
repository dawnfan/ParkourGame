#ifndef __GAMEWIN_H__
#define __GAMEWIN_H__

#include "cocos2d.h"

USING_NS_CC;

class GameWin : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameWin);
private:
	void BackFunc(cocos2d::Ref* pSender);
	void ReturnToMainFunc(cocos2d::Ref* pSender);
	void AboutFunc(cocos2d::Ref* pSender);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
};

#endif // __GAMEWIN_H__