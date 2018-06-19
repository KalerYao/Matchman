#include "Player.h"
#include "LuaUtil.h"
#include "OtherUtil.h"
#include <algorithm>
#include <iterator>

std::vector<std::string> Player::m_playerActionType;

std::vector<std::string> Player::m_endEvent;

void Player::LoadPlayerActionFromLua(const char* file)
{
	lua_State* pL = LuaUtil::openLuaFile(file);
	//��ȡ����
	//��ȡ�����б�����
	lua_getglobal(pL, "getPlayerActionTypeNum");
	lua_call(pL, 0, 1);
	int actionTypeNum = lua_tonumber(pL, -1);
	//��ȡ�����б�
	for (int i = 0;i < actionTypeNum;i++)
	{
		lua_getglobal(pL, "playerAction");
		lua_pushinteger(pL, i + 1);
		lua_gettable(pL, -2);
		const char* c = lua_tostring(pL, -1);
		m_playerActionType.push_back(Value(c).asString());
	}
	//��ȡ��������
	//��ȡ���������¼�����
	lua_getglobal(pL, "getEndEventNum");
	lua_call(pL, 0, 1);
	int endEventNum = lua_tonumber(pL, -1);
	//��ȡ���������¼�
	for (int i = 0;i < endEventNum;i++)
	{
		lua_getglobal(pL, "endEvent");
		lua_pushinteger(pL, i + 1);
		lua_gettable(pL, -2);
		const char* c = lua_tostring(pL, -1);
		m_endEvent.push_back(Value(c).asString());
	}

	LuaUtil::closeLuaFile(pL);
}

bool Player::TryTurnTo(std::string newAction)
{
	log("Now %s try turn to %s", m_nowAction.c_str(), newAction.c_str());
	if (m_isActionEnd)
	{
		log("%s is end", m_nowAction.c_str());
	}
	else
	{
		log("%s isn`t end", m_nowAction.c_str());
	}
	//��Action�����Ѵ�������
	if (!OtherUtil::isContain(m_playerActionType, newAction))
	{
		log("%s not exist!", newAction);
		return false;
	}

	//�鿴�Ƿ���Դ�nowActionת�䵽newAction
	if (!CanTurnTo(newAction))
	{
		log("this moment can`t turn %s to %s!", m_nowAction.c_str(), newAction.c_str());
		return false;
	}

	//����ת������
	log("Player %d:nowAction: %s", m_playerID,newAction.c_str());
	m_nowAction = newAction;

	//��run���⴦��
	//������
	if (m_nowAction == m_playerActionType.at(1))
	{
		m_armature->setScaleX(-1);
		m_armature->getAnimation()->play("run");
	}
	//������
	else if (m_nowAction == m_playerActionType.at(2))
	{
		m_armature->setScaleX(1);
		m_armature->getAnimation()->play("run");
	}
	else
	{
		m_armature->getAnimation()->play(m_nowAction);
	}

	//��nowAction�����жϣ��Ƿ�Ϊѭ��������������Ҫ�ȴ�����
	int nowActionID = getActionIDByActionType(m_nowAction);
	if (nowActionID > 2)
	{
		m_isActionEnd = false;
	}
	else
	{
		m_isActionEnd = true;
	}
	//����֡����
	if (m_isFrameOffset)
	{
		//m_isActionEnd = m_isActionEndoffset;
		m_isFrameOffset = false;
	}
	return true;
}

std::string Player::getPlayerActionTypeByID(int actionID)
{
	return m_playerActionType.at(actionID);
}

int Player::getActionIDByActionType(std::string actionType)
{
	std::vector<std::string>::iterator ite = find(m_playerActionType.begin(), m_playerActionType.end(), actionType);
	int actionID = std::distance(std::begin(m_playerActionType), ite);
	return actionID;
}

Player::Player()
{
}

Player::~Player()
{

}


Player* Player::create(int id, std::string type)
{
	Player *pRet = new(std::nothrow) Player();
		if (pRet && pRet->init(id,type)) 
		{ 
			pRet->autorelease(); 
			return pRet; 
		} 
		else 
		{ 
			delete pRet; 
			pRet = nullptr; 
			return nullptr; 
		} 
}

bool Player::init(int id, std::string type)
{
	if (!Sprite::init())
	{
		return false;
	}
	//��Ա��ֵ
	m_playerID = id;
	m_playerType = type;

	//���ó�ʼActionΪstand
	m_nowAction = m_playerActionType.at(0);

	//����playerTypeִ��lua�ű���ȡ�����ļ�
	lua_State* pL = LuaUtil::openLuaFile("luaData/testData.lua");
	lua_getglobal(pL, "getAnimationFile");
	lua_pushstring(pL, type.c_str());
	lua_call(pL, 1, 1);

	const char* c = lua_tostring(pL, -1);

	//��Ӷ���
	ArmatureDataManager::getInstance()->addArmatureFileInfo(c);
	m_armature = Armature::create("MatchmanTest");
	if (m_armature == NULL)
	{
		log("player %d animation load error!", m_playerID);
		return false;
	}
	m_armature->setPosition(Vec2::ZERO);
	m_armature->getAnimation()->play(m_playerActionType.at(0));
	this->setContentSize(m_armature->getContentSize());
	this->addChild(m_armature);

	m_isActionEnd = true;
	m_isActionEndoffset = true;
	m_isFrameOffset = false;

	//����֡����
	m_armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Player::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	return true;
}

bool Player::CanTurnTo(std::string newAction)
{

	if (!m_isActionEnd)
	{
		return false;
	}

	//�õ�2��action��ID
	int nowActionID = getActionIDByActionType(m_nowAction);
	int newActionID = getActionIDByActionType(newAction);

	if (nowActionID == newActionID)
	{
		return false;
	}

	return true;
}

void Player::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (OtherUtil::isContain(m_endEvent, evt))
	{
		m_isActionEnd = true;
		m_isActionEndoffset = true;
		m_isFrameOffset = true;
	}
}
