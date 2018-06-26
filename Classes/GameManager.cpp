#include "GameManager.h"
#include "PlayerType.h"
#include "Player.h"
#include "OtherUtil.h"
#include "MusicUtil.h"

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
	//���ʤ��������ʼ��
	m_finalNeedScore = 3;
	m_playerScore = new std::vector<int>();
	m_playerScore->push_back(0);
	m_playerScore->push_back(0);
	//�������ŷ�
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameManager::resetScene),
		"resetSceneInGameManager",
		NULL
	);
	//Ԥ�������� ���ű���
	MusicUtil::preLoadAllMusic();
	//���ű�������
	MusicUtil::playBackGroundMusic();
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

	//��ȡʤ��ID���÷�+1
	int winID = (int)pSender;
	m_playerScore->at(winID) += 1;
	//��������
	Director::getInstance()->replaceScene(m_GameScene);
	//���͸���score
	UIScore* uiScore = new UIScore();
	uiScore->id = winID;
	uiScore->score = m_playerScore->at(winID);
	NotificationCenter::getInstance()->postNotification("updateScore", uiScore);
	uiScore->id = 1-winID;
	uiScore->score = m_playerScore->at(1-winID);
	NotificationCenter::getInstance()->postNotification("updateScore", uiScore);
}
