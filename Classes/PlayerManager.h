/*
	��ҹ�����
	������ҽ�ɫ�����߼�
*/

#pragma once
#include "Player.h"

class PlayerManager
{
public:
	PlayerManager(Scene* sceneToAdd);
	~PlayerManager();
	Player *getPlayerByID(int i);
private:
	//���
	std::vector<Player*> m_players;
};