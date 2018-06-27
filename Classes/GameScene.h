/*
	��Ϸ������
	��Ϸ�����߼�����
*/

#pragma once
#include "cocos2d.h"
#include "PlayerManager.h"
#include "GameUI.h"

USING_NS_CC;

static std::string MAP_LAYER = "mainLayer";

class GameScene : public Layer
{
public:
	GameScene();
	~GameScene();
	static Scene* createScene(PlayerManager *playerManager);
    bool init() override;
	CREATE_FUNC(GameScene);
	static bool isTileCanbeStand(float x, float y);
	static void loadMapInfo(const char* file);
	void ShowWinBGInTime(float t);
private:
	//���б���
	Sprite* m_ultimateSkillBG;
	//ʤ������
	Sprite* m_winBG;
	//��ͼ��Ϣ����ͼ������ͼ�������ڼ�����ͼ
	static std::vector<std::string> m_mapNames;
	static int m_mapNum;
	static int m_mapOrder;
	static std::vector<std::vector<int>> m_canStandTileID;

	//����ͼ
	static std::vector<std::string> m_backgrounds;

	static TMXTiledMap *m_tileMap;

	PlayerManager *m_playerManager;

	void update(float dt) override;

	//���̼����¼�
	EventListenerKeyboard* m_listener;

	//��ͣ
	void Pause(Ref* pSender);

	//UI
	GameUI* m_ui;
	//���ŷ��� ����UI
	void UpdateUI(Ref* pSender);
	void UpdateScore(Ref* pSender);
	//���� playerʧ�ܸ��³���
	void ResetScene(Ref* pSender);
	//���� ���Ŵ��б���
	void ShowULBG(Ref* pSender);
};