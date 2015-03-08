#include "BackGround.h"

Scene* Background::createScene()
{
	auto scene = Scene::create();
	auto layer = Background::create();
	scene->addChild(layer);
	return scene;
}

bool Background::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	return true;
}