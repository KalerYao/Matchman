/*
	��Ϸ������
	��Ϸ�����߼�����
*/

#pragma once
#include "cocos2d.h"

USING_NS_CC;


class GameScene : public Layer
{
public:
	GameScene();
	~GameScene();

	static Scene* createScene();
    bool init() override;
	CREATE_FUNC(GameScene);
private:
	//��ͼ��Ϣ����ͼ������ͼ�������ڼ�����ͼ
	std::vector<std::string> m_mapNames;
	int m_mapNum;
	int m_mapOrder = 0;
};