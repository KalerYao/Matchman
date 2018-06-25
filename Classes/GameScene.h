/*
	��Ϸ������
	��Ϸ�����߼�����
*/

#pragma once
#include "cocos2d.h"
#include "PlayerManager.h"

USING_NS_CC;

static std::string MAP_LAYER = "mainLayer";

class GameScene : public Layer
{
public:
	GameScene();
	~GameScene();
	static Scene* createScene(PlayerManager playerManager);
    bool init() override;
	CREATE_FUNC(GameScene);
	static bool isTileCanbeStand(float x, float y);
private:
	//��ͼ��Ϣ����ͼ������ͼ�������ڼ�����ͼ
	static std::vector<std::string> m_mapNames;
	static int m_mapNum;
	static int m_mapOrder;
	static std::vector<int> m_canStandTileID;

	static TMXTiledMap *m_tileMap;

	PlayerManager m_playerManager;

	void update(float dt) override;
};