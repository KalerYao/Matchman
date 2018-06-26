/*
	��Ϸ������
	������Ϸ�����߼�
*/

#pragma once
#include "GameScene.h"
#include "PlayerManager.h"
#include <ctime>

class GameManager : public Ref
{
public:
	GameManager();
	~GameManager();

	//��ȡ��Ϸ����
	Scene* getGameScene();
		
private:

	Scene* m_GameScene;
	PlayerManager* m_playerManager;

	//��1��������������
	void resetScene(Ref* pSender);

	//��ʤ����Ҫ����
	int m_finalNeedScore;

	//2λ���ʤ������
	std::vector<int> *m_playerScore;
};