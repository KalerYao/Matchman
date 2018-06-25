#include "GameScene.h"
#include "LuaUtil.h"
#include "OtherUtil.h"
#include "PlayerManager.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{

}

Scene* GameScene::createScene(PlayerManager playerManage)
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	layer->m_playerManager = playerManage;
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//ִ��lua�ű���ȡ����
	lua_State* pL = LuaUtil::openLuaFile("luaData/testData.lua");

	//��ȡ��ͼ
	//��ȡ��ͼ����
	lua_getglobal(pL, "getMapsLen");
	lua_call(pL, 0, 1);
	m_mapNum = lua_tonumber(pL, -1);
	log("mapsNum = %d", m_mapNum);
	
	//��ȡ��ͼ��
	for (int i = 0;i < m_mapNum;i++)
	{
		lua_getglobal(pL, "maps");
		lua_pushinteger(pL, i + 1);
		lua_gettable(pL, -2);
		const char* m = lua_tostring(pL, -1);
		m_mapNames.push_back(Value(m).asString());
	}

	//��ȡ�����վ����ͼID
	lua_getglobal(pL, "getCanStandTileNum");
	lua_call(pL, 0, 1);
	int tileNum = lua_tointeger(pL, -1);

	for (int i = 0;i < tileNum;i++)
	{
		lua_getglobal(pL, "canStandTileID");
		lua_pushinteger(pL, i + 1);
		lua_gettable(pL, -2);
		m_canStandTileID.push_back(lua_tointeger(pL, -1));
	}

	//�ر�lua�ű�
	LuaUtil::closeLuaFile(pL);

	//���ݵ�ͼ����ȡ�ļ�
	m_tileMap = TMXTiledMap::create(m_mapNames.at(m_mapOrder));
	this->addChild(m_tileMap);

	//ע����̼�������
	auto listener = EventListenerKeyboard::create();
	//���尴������
	listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event * event) {
		//���밴�°���Ϊ�Ѷ��尴��
		for (int i = 0;i < PlayerManager::getAllKey().size();i++)
		{
			if (OtherUtil::isContain(PlayerManager::getAllKey().at(i), keyCode))
			{
				m_playerManager.DoActionByKeyCode(i, keyCode);
				//����
				if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 1)
				{
					m_playerManager.changePlayerRunDirByID(i, -1);
				}
				//����
				else if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 2)
				{
					m_playerManager.changePlayerRunDirByID(i, 1);
				}
				if (PlayerManager::GetKeyIDByKeyCode(keyCode) != 4 && PlayerManager::GetKeyIDByKeyCode(keyCode) != 10)
				{
					m_playerManager.ReLoadActionByID(i);
				}
				return;
			}
		}
	};
	//���尴���ɿ�����ִ�����ߵȶ���ʱ����ֱ�������ִ��stand���������ɽ���ʱӦ�õȴ����������ִ��stand
	listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event * event) {
		//����ɿ��İ���Ϊ�ĸ�player��
		for (int i = 0;i < PlayerManager::getAllKey().size();i++)
		{
			if (OtherUtil::isContain(PlayerManager::getAllKey().at(i), keyCode))
			{
				//֮ǰ����
				if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 1)
				{
					m_playerManager.changePlayerRunDirByID(i, 1);
				}
				//֮ǰ����
				else if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 2)
				{
					m_playerManager.changePlayerRunDirByID(i, -1);
				}
				m_playerManager.ReLoadActionByID(i);
				return;
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//��Ӷ�ʱ��
	this->scheduleUpdate();

	return true;
}

bool GameScene::isTileCanbeStand(float x, float y)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//����ID
	int mapx = x / m_tileMap->getTileSize().width;
	mapx = mapx >= 0 ? mapx : 0;
	int mapy = (visibleSize.height - y) / m_tileMap->getTileSize().height;
	mapy = mapy >= 0 ? mapy : 0;
	if (OtherUtil::isContain(m_canStandTileID,int(m_tileMap->getLayer(MAP_LAYER)->getTileGIDAt(Vec2(mapx, mapy+1)))-1))
	{
		return true;
	}
	return false;
}

std::vector<std::string> GameScene::m_mapNames;

int GameScene::m_mapNum;

int GameScene::m_mapOrder;

std::vector<int> GameScene::m_canStandTileID;

TMXTiledMap * GameScene::m_tileMap;

void GameScene::update(float dt)
{
	//����Ƿ��й������ܵ�����
	if (m_playerManager.IsPlayersCollide())
	{
		//log("collosion!");
		//if (m_playerManager.GetBattleStateByID(0) != NONE || m_playerManager.GetBattleStateByID(1) != NONE)
		//{
		//	//test
		//	//log("test");
		//	bool a = m_playerManager.isPlayer1Left();
		//	int b = m_playerManager.GetTowardsByID(0);
		//	int c = m_playerManager.GetTowardsByID(1);
		//	int d = b;
		//}
		//�κ�1�������򲻻�������
		if (m_playerManager.GetBattleStateByID(0) == HURT || m_playerManager.GetBattleStateByID(1) == HURT)
		{
			return;
		}

		//���1�Ƿ�����2����˺�
		if (m_playerManager.isPlayer1Left() && m_playerManager.GetTowardsByID(0) == 1
			|| !m_playerManager.isPlayer1Left() && m_playerManager.GetTowardsByID(0) == -1)
		{
			if (m_playerManager.GetBattleStateByID(0) == ATTACK)
			{
				m_playerManager.ForceToHurtByID(1, Player::GetKickHurt());
			}
			else if (m_playerManager.GetBattleStateByID(0) == UATTACK)
			{
				m_playerManager.ForceToHurtByID(1, Player::GetUltimateSkillHurt());
			}
		}
		//���2�����1����˺�
		if (m_playerManager.isPlayer1Left() && m_playerManager.GetTowardsByID(1) == -1
			|| !m_playerManager.isPlayer1Left() && m_playerManager.GetTowardsByID(1) == 1)
		{
			if (m_playerManager.GetBattleStateByID(1) == ATTACK)
			{
				m_playerManager.ForceToHurtByID(0, Player::GetKickHurt());
			}
			else if (m_playerManager.GetBattleStateByID(1) == UATTACK)
			{
				m_playerManager.ForceToHurtByID(0, Player::GetUltimateSkillHurt());
			}
		}
	}
}