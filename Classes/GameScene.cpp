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
	//�ر�lua�ű�
	LuaUtil::closeLuaFile(pL);

	//���ݵ�ͼ����ȡ�ļ�
	auto tileMap = TMXTiledMap::create(m_mapNames.at(m_mapOrder));
	this->addChild(tileMap);

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
				m_playerManager.ReLoadActionByID(i);
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
	return true;
}

std::vector<std::string> GameScene::m_mapNames;

int GameScene::m_mapNum;

int GameScene::m_mapOrder;

void GameScene::update(float dt)
{
	if (m_playerManager.IsPlayersCollide())
	{
		log("player collide!");
	}
}
