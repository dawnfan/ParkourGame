#include "BackGround.h"


bool Background::init()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	bgSprite = Sprite::create("Map00.png");
	//bgSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	//groundSprite = Sprite::create("Ground00.png");
	this->addChild(bgSprite);
	//this->addChild(groundSprite);
	return true;
}
