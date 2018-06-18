#include "PlayerManager.h"

PlayerManager::PlayerManager(Scene* sceneToAdd)
{
	//д������
	m_players.push_back(Player::create(0, PlayerType::GetPlayerTypeByOrder(0)));
	m_players.push_back(Player::create(1, PlayerType::GetPlayerTypeByOrder(1)));

	log("Scene Tag : %d", sceneToAdd->getTag());
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//�������ӽ�����
	for (int i = 0;i < m_players.size();i++)
	{
		sceneToAdd->addChild(m_players.at(i));
		m_players.at(i)->setPosition(visibleSize.width / 4 * (i+2), visibleSize.height / 2);
	}

}

PlayerManager::~PlayerManager()
{

}

Player *PlayerManager::getPlayerByID(int i)
{
	return m_players.at(i);
}
