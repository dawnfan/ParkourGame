#ifndef __POPUP_H__
#define __POPUP_H__

#include "cocos2d.h"

USING_NS_CC;

class Popup : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();  
	CREATE_FUNC(Popup);
private:
	void BackFunc(cocos2d::Ref* pSender);
	void ReturnToMainFunc(cocos2d::Ref* pSender);
	void AboutFunc(cocos2d::Ref* pSender);
	void onKeyReleased(EventKeyboard::KeyCode keycode,Event* event);
};

#endif // __POPUP_H__