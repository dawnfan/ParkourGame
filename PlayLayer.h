#ifndef __PLAYLAYER_H__
#define __PLAYLAYER_H__

#include "cocos2d.h"
#include "Runner.h"
#include "Manager.h"
#include "GameLayer.h"

#define BACKGROUND_SPEED 4
USING_NS_CC;

class PlayLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(PlayLayer);

	PlayLayer();

	virtual bool onTouchBegan(Touch *touch, Event *unused) override;
	virtual void onTouchMoved(Touch *touch, Event *unused) override;
	bool onContactBegin(PhysicsContact& contact);
	//���ؼ���Ӧ
	void onKeyReleased(EventKeyboard::KeyCode keycode,Event* event);
	void setPhyWorld(PhysicsWorld* world){ m_world = world; }

	//��update�����õ�ͼ����
	virtual void update(float dt);

	void setLevel(unsigned lev);
	void setTarget(unsigned tar);
	ProgressTimer* getProbar();
	Sprite* getHaze();

private:
	//��������
	PhysicsWorld* m_world;
	//�洢���е�֡ͼƬ
	Vector<SpriteFrame* >allframes;
	Animation* animation;
	Animate* animate;
	//���ܵ�Ҷ��
	Runner* hero;
	//��������
	Sprite* bgSprite1;
	Sprite* bgSprite2;
	Sprite* groundSprite1;
	Sprite* groundSprite2;
	//���ڹ����ҵ�
	Manager* man;
	//������ʼ��
	int startPoint;
	//�÷֣������Ƿ����
	unsigned score;
	//�����ɰ�
	Sprite* haze;
	//��ǰ����
	unsigned level;
	//����Ŀ�����
	unsigned target;
	//��ʾ����ǰ������Ŀ��������÷�
	LabelTTF* m_level;
	LabelTTF* m_target;
	LabelTTF* m_score;
	//������
	ProgressTimer* proBar;


	//�Խ��������Ч
	void runEffect(Sprite* coin);
	//��ʼ������
	void initBG();
};

#endif // __PLAYLAYER_H__
