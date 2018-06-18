#include "GameManager.h"
#include "PlayerType.h"

GameManager::GameManager()
{
	//�ӽű���ȡ�������
	PlayerType::LoadTypeFromLua();
	//������Ϸ����
	m_GameScene = GameScene::createScene();
	m_GameScene->setTag(9);
	//������ҹ�����
	m_playerManager = new PlayerManager(m_GameScene);
}

GameManager::~GameManager()
{

}

Scene* GameManager::getGameScene()
{
	return m_GameScene;
}
