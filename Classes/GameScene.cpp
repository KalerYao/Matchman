#include "GameScene.h"
#include "LuaUtil.h"
#include "OtherUtil.h"
#include "PlayerManager.h"
#include "LoadScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

Scene* GameScene::createScene(PlayerManager *playerManage)
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	layer->setName("gameScene");
	layer->m_playerManager = playerManage;
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//���ʤ������
	m_winBG = Sprite::create("background/WinBG.png");
	this->addChild(m_winBG, 5);
	m_winBG->runAction(FadeOut::create(0.0F));
	m_winBG->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	//���UI��
	m_ui = GameUI::create();
	this->addChild(m_ui, 3);
	//�������ŷ����������Ѫ��������
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameScene::UpdateUI),
		"updateUI",
		NULL
	);
	//������ҵ÷�
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameScene::UpdateScore),
		"updateScore",
		NULL
	);
	//������ŷ����������ʤ��
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameScene::ResetScene),
		"resetSceneInGameScene",
		NULL
	);
	//������ţ�������ͣ�¼�
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameScene::Pause),
		"pause",
		NULL
	);
	//������ţ���������
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameScene::ShowULBG),
		"showULBG",
		NULL
	);


	//�����ӱ���ͼ
	int backOrder = rand() % m_backgrounds.size();
	std::string path = m_backgrounds.at(backOrder);
	auto bg = Sprite::create(path);
	this->addChild(bg, 1);
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	//���ѡ����ͼ
	m_mapOrder = rand() % m_mapNum;

	//���ݵ�ͼ����ȡ�ļ�
	m_tileMap = TMXTiledMap::create(m_mapNames.at(m_mapOrder));
	this->addChild(m_tileMap,2);

	//��Ӵ��б���
	m_ultimateSkillBG = Sprite::create("background/UlimateSkillBG.png");
	m_ultimateSkillBG->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(m_ultimateSkillBG, 5);
	m_ultimateSkillBG->setVisible(false);

	//ע����̼�������
	m_listener = EventListenerKeyboard::create();
	//���尴������
	m_listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event * event) {
		//���밴�°���Ϊ�Ѷ��尴��
		for (int i = 0;i < PlayerManager::getAllKey().size();i++)
		{
			if (OtherUtil::isContain(PlayerManager::getAllKey().at(i), keyCode))
			{
				m_playerManager->DoActionByKeyCode(i, keyCode);
				//����
				if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 1)
				{
					m_playerManager->changePlayerRunDirByID(i, -1);
					log("left press!");
				}
				//����
				else if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 2)
				{
					m_playerManager->changePlayerRunDirByID(i, 1);
					log("right press!");
				}
				if (PlayerManager::GetKeyIDByKeyCode(keyCode) != 4 && PlayerManager::GetKeyIDByKeyCode(keyCode) != 10)
				{
					m_playerManager->ReLoadActionByID(i);
				}
				return;
			}
		}
	};
	//���尴���ɿ�����ִ�����ߵȶ���ʱ����ֱ�������ִ��stand���������ɽ���ʱӦ�õȴ����������ִ��stand
	m_listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event * event) {
		//����ɿ��İ���Ϊ�ĸ�player��
		for (int i = 0;i < PlayerManager::getAllKey().size();i++)
		{
			if (OtherUtil::isContain(PlayerManager::getAllKey().at(i), keyCode))
			{
				//֮ǰ����
				if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 1)
				{
					m_playerManager->changePlayerRunDirByID(i, 1);
					log("left release!");
				}
				//֮ǰ����
				else if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 2)
				{
					m_playerManager->changePlayerRunDirByID(i, -1);
					log("right release!");
				}
				m_playerManager->ReLoadActionByID(i);
				return;
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);

	//��Ӷ�ʱ��
	this->scheduleUpdate();

	return true;
}

bool GameScene::isTileCanbeStand(float x, float y)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//����ID
	int mapx = (x - m_tileMap->getTileSize().width/2) / m_tileMap->getTileSize().width;
	mapx = mapx >= 0 ? mapx : 0;
	int mapy = (visibleSize.height - y) / m_tileMap->getTileSize().height;
	mapy = mapy >= 0 ? mapy : 0;
	if (OtherUtil::isContain(m_canStandTileID.at(m_mapOrder),int(m_tileMap->getLayer(MAP_LAYER)->getTileGIDAt(Vec2(mapx, mapy+1)))-1))
	{
		return true;
	}
	return false;
}

void GameScene::loadMapInfo(const char* file)
{
	//ִ��lua�ű���ȡ����
	lua_State* pL = LuaUtil::openLuaFile(file);

	//��ȡ����ͼ
	lua_getglobal(pL, "getBackGroundNum");
	lua_call(pL, 0, -1);
	int backNum = lua_tointeger(pL, -1);

	for (int i = 0;i < backNum;i++)
	{
		lua_getglobal(pL, "background");
		lua_pushinteger(pL, i + 1);
		lua_gettable(pL, -2);
		const char* c = lua_tostring(pL, -1);
		m_backgrounds.push_back(Value(c).asString());
	}

	//��ȡ��ͼ
	//��ȡ��ͼ����
	lua_getglobal(pL, "getMapsLen");
	lua_call(pL, 0, 1);
	m_mapNum = lua_tonumber(pL, -1);

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
	for (int i = 0;i < m_mapNum;i++)
	{
		m_canStandTileID.push_back(std::vector<int>());

		lua_getglobal(pL, "getCanStandTileNumByID");
		lua_pushinteger(pL, i);
		lua_call(pL, 1, 1);
		int tileNum = lua_tointeger(pL, -1);

		for (int j = 0;j < tileNum;j++)
		{
			lua_getglobal(pL, "canStandTileID");
			lua_pushinteger(pL, i + 1);
			lua_gettable(pL, -2);

			lua_pushinteger(pL, j + 1);
			lua_gettable(pL, -2);
			m_canStandTileID.at(i).push_back(lua_tointeger(pL, -1));
		}
	}

	LuaUtil::closeLuaFile(pL);
}

void GameScene::ShowWinBGInTime(float t)
{
	m_winBG->runAction(FadeIn::create(t));
	this->runAction(Sequence::create(DelayTime::create(t+1.5F), CallFunc::create([]() {
		Director::getInstance()->replaceScene(LoadScene::createScene());
	}), nullptr));
}

std::vector<std::string> GameScene::m_mapNames;

int GameScene::m_mapNum;

int GameScene::m_mapOrder;

std::vector<std::vector<int>> GameScene::m_canStandTileID;

std::vector<std::string> GameScene::m_backgrounds;

TMXTiledMap * GameScene::m_tileMap;

void GameScene::update(float dt)
{
	//����Ƿ��й������ܵ�����
	if (m_playerManager->IsPlayersCollide())
	{
		//�κ�1�������򲻻�������
		if (m_playerManager->GetBattleStateByID(0) == HURT || m_playerManager->GetBattleStateByID(1) == HURT)
		{
			return;
		}

		//���1�Ƿ�����2����˺�
		if (m_playerManager->isPlayer1Left() && m_playerManager->GetTowardsByID(0) == 1
			|| !m_playerManager->isPlayer1Left() && m_playerManager->GetTowardsByID(0) == -1)
		{
			if (m_playerManager->GetBattleStateByID(0) == ATTACK)
			{
				m_playerManager->ForceToHurtByID(1, Player::GetKickHurt());
				log("1 hurt 2");
			}
			else if (m_playerManager->GetBattleStateByID(0) == UATTACK)
			{
				m_playerManager->ForceToHurtByID(1, Player::GetUltimateSkillHurt());
			}
		}
		//���2�����1����˺�
		if (m_playerManager->isPlayer1Left() && m_playerManager->GetTowardsByID(1) == -1
			|| !m_playerManager->isPlayer1Left() && m_playerManager->GetTowardsByID(1) == 1)
		{
			if (m_playerManager->GetBattleStateByID(1) == ATTACK)
			{
				m_playerManager->ForceToHurtByID(0, Player::GetKickHurt());
			}
			else if (m_playerManager->GetBattleStateByID(1) == UATTACK)
			{
				m_playerManager->ForceToHurtByID(0, Player::GetUltimateSkillHurt());
			}
		}
	}
}

void GameScene::Pause(Ref* pSender)
{
	if (!Director::getInstance()->isPaused())
	{
		Director::getInstance()->pause();
		_eventDispatcher->pauseEventListenersForTarget(this);
	}
	else
	{
		Director::getInstance()->resume();
		_eventDispatcher->resumeEventListenersForTarget(this);
	}
}

void GameScene::UpdateUI(Ref* pSender)
{
	UIMessage* uiMessage = (UIMessage*)pSender;
	m_ui->SetHealthByID(uiMessage->id, uiMessage->health);
	m_ui->SetPowerByID(uiMessage->id, uiMessage->power);
}

void GameScene::UpdateScore(Ref* pSender)
{
	UIScore* uiScore = (UIScore*)pSender;
	m_ui->SetScoreByID(uiScore->id, uiScore->score);
}

void GameScene::ResetScene(Ref* pSender)
{
	//���µ÷�
	int failID = (int)pSender;
	int winID = 1 - failID;
	//���Ƚ�����̿���
	_eventDispatcher->removeAllEventListeners();
	//��ֹͣ����ж�
	m_playerManager->changePlayerRunDirByID(winID, -(m_playerManager->getPlayerAtIDRunDir(winID)-1));
	m_playerManager->ReLoadActionByID(winID);

	auto sendMsg = CallFunc::create([=]() {
		NotificationCenter::getInstance()->postNotification("resetSceneInGameManager", (Ref*)winID);
	});

	//����Ӯ�����
	this->runAction(Sequence::create(DelayTime::create(2.0F), sendMsg, nullptr));
}

void GameScene::ShowULBG(Ref* pSender)
{
	m_ultimateSkillBG->setVisible(true);
	m_ultimateSkillBG->runAction(Sequence::create(DelayTime::create(2.0F), CallFunc::create([&]() {
		m_ultimateSkillBG->setVisible(false);
	}), nullptr));
}
