#ifndef __ENDLAYER_H__
#define __ENDLAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class EndLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();  
	CREATE_FUNC(EndLayer);
	EndLayer();
	//根据win的真假设置背景和按钮
	void setNext(bool win,unsigned n_lev,unsigned n_tar);
	LabelTTF* end_score;
	LabelTTF* target;
	LabelTTF* level;
	LabelTTF* prompt;

	void setCatchedDrop();
private:
	Sprite* end;
	unsigned n_level;
	unsigned n_target;
	LabelTTF* haze;
	unsigned hazeCount;
	void NextFunc(cocos2d::Ref* pSender);
	void RetryFunc(cocos2d::Ref* pSender);
	void ReturnToMainFunc(cocos2d::Ref* pSender);
	void onKeyReleased(EventKeyboard::KeyCode keycode,Event* event);
	void hazeUpdate(float dt);
	bool catchedDrop;
};

#endif // __ENDLAYER_H__
