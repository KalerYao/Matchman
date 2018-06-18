#include "GameScene.h"
#include "LuaUtil.h"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{

}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
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

	//������

	return true;
}
