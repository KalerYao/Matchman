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
	//�ӽű���ȡ��ͼ��Ϣ
	GameScene::loadMapInfo("luaData/testData.lua");

	//���������������
	srand(time(NULL));

	//������ҹ�����
	m_playerManager = new PlayerManager();
	//������Ϸ����
	m_GameScene = GameScene::createScene(m_playerManager);
	m_playerManager->AddPlayerToScene(m_GameScene);

	//�������ŷ�
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameManager::resetScene),
		"resetScene",
		NULL
	);
}

GameManager::~GameManager()
{
	//�Ƴ�����
	NotificationCenter::getInstance()->removeAllObservers(this);
}

Scene* GameManager::getGameScene()
{
	return m_GameScene;
}

void GameManager::resetScene(Ref* pSender)
{
	m_GameScene->cleanup();
	m_playerManager->ReloadPlayer();
	m_GameScene = GameScene::createScene(m_playerManager);
	m_playerManager->AddPlayerToScene(m_GameScene);
	//��������
	//auto delay = DelayTime::create(2.0F);
	//auto replaceScene = CallFunc::create([&]() {
	//	Director::getInstance()->replaceScene(m_GameScene);
	//});
	//this->runAction(Sequence::create(delay, replaceScene, nullptr));
	Director::getInstance()->replaceScene(m_GameScene);
}
