#include "GameManager.h"
#include "PlayerType.h"
#include "Player.h"

GameManager::GameManager()
{
	//�ӽű���ȡ�������
	PlayerType::LoadTypeFromLua("luaData/testData.lua");
	//�ӽű���ȡ��Ҷ�������
	Player::LoadPlayerActionFromLua("luaData/testData.lua");
	//�ӽű���ȡ��ҵİ����Ͷ����Ķ�Ӧ��ϵ
	PlayerManager::LoadActionKeyAndIDFromLua("luaData/testData.lua");
	//������ҹ�����
	m_playerManager = new PlayerManager();
	//������Ϸ����
	m_GameScene = GameScene::createScene(*m_playerManager);
	m_playerManager->AddPlayerToScene(m_GameScene);
}

GameManager::~GameManager()
{

}

Scene* GameManager::getGameScene()
{
	return m_GameScene;
}
