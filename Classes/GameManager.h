/*
	��Ϸ������
	������Ϸ�����߼�
*/

#pragma once
#include "GameScene.h"
#include "PlayerManager.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	//��ȡ��Ϸ����
	Scene* getGameScene();
		
private:
	Scene* m_GameScene;
	PlayerManager* m_playerManager;
};