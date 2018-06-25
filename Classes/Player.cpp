#include "Player.h"
#include "LuaUtil.h"
#include "OtherUtil.h"
#include "PlayerManager.h"
#include "GameScene.h"
#include <algorithm>
#include <iterator>

//����
void setPositive(RunDir &r)
{
	switch (r)
	{
	case RUNLEFT:
		r = RunDir::STAND;
		break;
	case STAND:
		r = RunDir::RUNRIGHT;
		break;
	case RUNRIGHT:
		r = RunDir::RUNRIGHT;
		break;
	default:
		break;
	}
}

//����
void setNegetive(RunDir &r)
{
	switch (r)
	{
	case RUNLEFT:
		r = RunDir::RUNLEFT;
		break;
	case STAND:
		r = RunDir::RUNLEFT;
		break;
	case RUNRIGHT:
		r = RunDir::STAND;
		break;
	default:
		break;
	}
}

std::vector<std::string> Player::m_playerActionType;

float Player::m_playerMoveUnit;

float Player::m_playerJumpUnit;

int Player::m_maxHealth;

int Player::m_maxUltimateSlillNeed;

int Player::m_punchHurt;

int Player::m_kickHurt;

int Player::m_ultimateSkillHurt;

int Player::m_addPower;

std::vector<std::string> Player::m_endEvent;

void Player::update(float dt)
{
	//����λ��
	int actionID = getActionIDByActionType(m_nowAction);
	switch (actionID)
	{
	case 1:
		//����
		m_armature->setPositionX(m_armature->getPositionX() - m_playerMoveUnit);
		break;
	case 2:
		//����
		m_armature->setPositionX(m_armature->getPositionX() + m_playerMoveUnit);
		break;
	case 3:
		//��Ծ
		m_armature->setPositionY(m_armature->getPositionY() + m_playerJumpUnit * 5 / 4);
		switch (m_runDir)
		{
		case RUNLEFT:
			m_armature->setPositionX(m_armature->getPositionX() - m_playerMoveUnit);
			break;
		case STAND:
			break;
		case RUNRIGHT:
			m_armature->setPositionX(m_armature->getPositionX() + m_playerMoveUnit);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	//����Ƿ񳬳���Χ
	checkRange();

	//����Ƿ���Ҫ����
	if (!GameScene::isTileCanbeStand(m_armature->getPositionX(), m_armature->getPositionY()))
	{
		m_canstand = false;
		m_armature->setPositionY(m_armature->getPositionY() - m_playerJumpUnit * 3 / 4);
		//����״̬
		if (actionID != 3)
		{
			m_isFall = true;
		}
	}
	else
	{
		if (m_canstand == false)
		{
			//�ж��ܶ�����
			Player::ReLoadAction();
		}
		m_canstand = true;
		if (actionID != 4 && actionID != 10 && actionID != 11)
		{
			if (m_isFall = true)
			{
				//�ж��ܶ�����
				Player::ReLoadAction();
			}
			m_isFall = false;
		}
	}



	//drawNode->clear();
	//Vec2 o = m_armature->getBoundingBox().origin;
	//Vec2 s = m_armature->getBoundingBox().size;
	//auto pos = this->getPosition();
	//auto box = this->getBoundingBox();
	//drawNode->drawRect(m_armature->getBoundingBox().origin, m_armature->getBoundingBox().origin + m_armature->getBoundingBox().size, Color4F(0.0F, 1.0F, 0.0F, 1.0F));
	//drawNode->drawRect(this->getBoundingBox().origin, this->getBoundingBox().origin + this->getBoundingBox().size, Color4F(1.0F, 0.0F, 0.0F, 1.0F));


	//pos = this->getPosition();
	//box = this->getBoundingBox();
}

void Player::checkRange()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//����Ƿ񳬳���Χ
	if (m_armature->getPositionX() > visibleSize.width-10)
	{
		m_armature->setPositionX(visibleSize.width-10);
	}
	if (m_armature->getPositionX() < 0)
	{
		m_armature->setPositionX(0);
	}
	if (m_armature->getPositionY() > visibleSize.height)
	{
		m_armature->setPositionY(visibleSize.height);
	}
	if (m_armature->getPositionY() < 0)
	{
		m_armature->setPositionY(0);
	}
}

void Player::SendToUpateUI()
{
	UIMessage* uiMessage = new UIMessage();
	uiMessage->id = m_playerID;
	uiMessage->health = m_nowHealth;
	uiMessage->power = m_nowPower;
	NotificationCenter::getInstance()->postNotification("updateUI", uiMessage);
}

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

	//��ȡ����ƶ���λ
	lua_getglobal(pL, "playerMoveUnit");
	m_playerMoveUnit = lua_tonumber(pL, -1);
	//��ȡ�����Ծ��λ
	lua_getglobal(pL, "playerJumpUnit");
	m_playerJumpUnit = lua_tonumber(pL, -1);

	//��ȡ��ҽ�����������Ϣ
	lua_getglobal(pL, "playerHealth");
	m_maxHealth = lua_tointeger(pL, -1);

	lua_getglobal(pL, "playerultimateSkillNeed");
	m_maxUltimateSlillNeed = lua_tointeger(pL, -1);

	lua_getglobal(pL, "punchHurt");
	m_punchHurt = lua_tointeger(pL, -1);

	lua_getglobal(pL, "kickHurt");
	m_kickHurt = lua_tointeger(pL, -1);

	lua_getglobal(pL, "ultimateSkillHurt");
	m_ultimateSkillHurt = lua_tointeger(pL, -1);

	lua_getglobal(pL, "addPower");
	m_addPower = lua_tointeger(pL, -1);

	LuaUtil::closeLuaFile(pL);
}

bool Player::TryTurnTo(std::string newAction)
{
	//�鿴�Ƿ���Դ�nowActionת�䵽newAction
	if (!CanTurnTo(newAction))
	{
		//log("this moment can`t turn %s to %s!", m_nowAction.c_str(), newAction.c_str());
		return false;
	}



	//����״̬
	m_nowAction = newAction;

	int nowActionID = getActionIDByActionType(m_nowAction);

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

	//�ж�ս��״̬
	if (getActionIDByActionType(m_nowAction) < 5 || getActionIDByActionType(m_nowAction) == 7 || 
		getActionIDByActionType(m_nowAction) == 9)
	{
		m_battleState = NONE;
	}
	else if (getActionIDByActionType(m_nowAction) == 5
		|| getActionIDByActionType(m_nowAction) == 6)
	{
		m_battleState = ATTACK;
	}
	else if (getActionIDByActionType(m_nowAction) == 8)
	{
		m_battleState = UATTACK;
	}
	//��nowAction�����жϣ��Ƿ�Ϊѭ��������������Ҫ�ȴ�����
	if (nowActionID > 2 && nowActionID != 4 && nowActionID != 10)
	{
		m_isActionEnd = false;
	}
	else
	{
		m_isActionEnd = true;
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

RunDir Player::getRunDir()
{
	return m_runDir;
}



void Player::changeRunDir(int change)
{
	if (change > 0)
	{
		setPositive(m_runDir);
	}
	else if (change < 0)
	{
		setNegetive(m_runDir);
	}
	log("run dir: %d", m_runDir);
}

bool Player::isActionEnd()
{
	return m_isActionEnd;
}

void Player::ReLoadAction()
{
	//���ݵ�ǰ�ܶ��������
	switch (m_runDir)
	{
	case STAND:
		TryTurnTo(m_playerActionType.at(0));
		break;
	case RUNLEFT:
		TryTurnTo(m_playerActionType.at(1));
		break;
	case RUNRIGHT:
		TryTurnTo(m_playerActionType.at(2));
		break;
	default:
		break;
	}
}

int Player::GetTowards()
{
	return m_armature->getScaleX();
}

BattleState Player::GetBattleState()
{
	return m_battleState;
}

void Player::ForceToHurt(int damage)
{
	//�����¶ף���Ծ״̬���߷���״̬���������˶���
	//��Ծ
	if (getActionIDByActionType(m_nowAction) == 3)
	{
		m_nowHealth -= damage;
	}
	//�¶׻���Ծ
	else if (getActionIDByActionType(m_nowAction) == 4 || getActionIDByActionType(m_nowAction) == 10)
	{
		m_nowHealth -= damage / 2;
	}
	else
	{
		m_nowHealth -= damage;
		m_armature->getAnimation()->play(m_playerActionType.at(9));
		m_isActionEnd = false;
	}

	m_battleState = HURT;
	m_nowAction = m_playerActionType.at(9);


	//���Ÿ���ui��Ϣ
	SendToUpateUI();

	if (m_nowHealth <= 0)
	{
		m_nowHealth = 0;
		m_armature->getAnimation()->play(m_playerActionType.at(11));
		m_isActionEnd = false;
		m_nowAction = m_playerActionType.at(11);
	}
	log("Player: %d,Health: %d,Power: %d", m_playerID, m_nowHealth, m_nowPower);
}

float Player::GetAnimationPositionX()
{
	return m_armature->getPositionX();
}

Sprite* Player::getAnimature()
{
	return (Sprite*)m_armature;
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
	if (pRet && pRet->init(id, type))
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
	m_armature = Armature::create("MatchmanAnimation");
	if (m_armature == NULL)
	{
		log("player %d animation load error!", m_playerID);
		return false;
	}
	m_armature->getAnimation()->play(m_playerActionType.at(0));
	this->setContentSize(m_armature->getContentSize());
	this->addChild(m_armature);
	
	m_isActionEnd = true;

	//�����ǵ���ID�������
	if (m_playerID % 2 == 1)
	{
		m_armature->setScaleX(-1);
	}

	//����֡����
	m_armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Player::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	//��ʼ��λ��
	float x, y;
	lua_getglobal(pL, "playersPos");
	lua_pushnumber(pL, m_playerID + 1);
	lua_gettable(pL, -2);
	lua_pushnumber(pL, 1);
	lua_gettable(pL, -2);
	x = lua_tonumber(pL, -1);
	lua_getglobal(pL, "playersPos");
	lua_pushnumber(pL, m_playerID + 1);
	lua_gettable(pL, -2);
	lua_pushnumber(pL, 2);
	lua_gettable(pL, -2);
	y = lua_tonumber(pL, -1);
	//this->setPosition(Vec2(x, y));


	//Ϊ�˱��ڼ�����ש��ID����ê��
	//this->setAnchorPoint(Vec2(0.5F, 1.0F));

	//test
	//m_armature->setPosition(this->getPosition());
	//m_armature->setAnchorPoint(Vec2::ZERO);

	m_armature->setPosition(Vec2(x, y));
	m_armature->setAnchorPoint(Vec2(0.5F, 0.0F));
	//��ʼ���ܶ�����
	m_runDir = RunDir::STAND;

	//���ö�ʱ��
	this->scheduleUpdate();

	//������Ծ������״̬
	m_isJump = false;
	m_isFall = true;

	//���ó�ʼ������
	m_canstand = false;

	//��ʼ����������
	m_nowHealth = m_maxHealth;
	m_nowPower = 0;

	//��ʼ��ս��״̬
	m_battleState = NONE;

	//�ر�lua�ű�
	LuaUtil::closeLuaFile(pL);

	//��������Ч��
	m_hurtEffect = ParticleSystemQuad::create("particleSystem/collide.plist");
	this->addChild(m_hurtEffect);
	m_hurtEffect->setAutoRemoveOnFinish(false);
	m_hurtEffect->setVisible(false);
	m_hurtEffect->setScale(0.5F);
	//���ô���ʱ���ͷ�����
	CallFunc *hideParticleFirst = CallFunc::create([&]() {
		m_hurtEffect->setVisible(true);
	});
	DelayTime *hideDuringTime = DelayTime::create(0.5F);
	this->runAction(Sequence::create(hideDuringTime, hideParticleFirst, nullptr));

	//test
	drawNode = DrawNode::create();
	this->addChild(drawNode);
	return true;
}

bool Player::CanTurnTo(std::string newAction)
{
	//��Action�����Ѵ�������
	if (!OtherUtil::isContain(m_playerActionType, newAction))
	{
		log("%s not exist!", newAction);
		return false;
	}

	//��������״̬
	if (getActionIDByActionType(newAction) == 11)
	{
		return false;
	}

	//����û���꣬�������� ����ת��״̬
	if (!m_isActionEnd || m_isFall)
	{
		return false;
	}

	//���粻�������ͷŴ���
	if (getActionIDByActionType(newAction) == 8 && m_nowPower < m_maxUltimateSlillNeed)
	{
		log("Not enough power!");
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
	if (strcmp(evt.c_str(), "damage_end") == 0)
	{
		m_battleState = NONE;
	}
	if (OtherUtil::isContain(m_endEvent, evt))
	{
		m_isActionEnd = true;
		//λ�øı�
		//������˲����Ҫ�ı�λ��
		if (getActionIDByActionType(m_nowAction) == 7)
		{
			m_armature->setPositionX(m_armature->getPositionX() + m_armature->getScaleX() * 56);
			checkRange();
		}
		//����Ǵ���ҲҪ�ı�λ��
		else if (getActionIDByActionType(m_nowAction) == 8)
		{
			m_armature->setPositionX(m_armature->getPositionX() + m_armature->getScaleX() * 375);
			checkRange();
		}

		//ʹ�ü�����������
		if (getActionIDByActionType(m_nowAction) == 5 || getActionIDByActionType(m_nowAction) == 6 || getActionIDByActionType(m_nowAction) == 7)
		{
			m_nowPower += m_addPower;
			m_nowPower = m_nowPower > m_maxUltimateSlillNeed ? m_maxUltimateSlillNeed : m_nowPower;

			SendToUpateUI();
		}

		//ʹ�ô�����������
		if (getActionIDByActionType(m_nowAction) == 8)
		{
			m_nowPower = 0;
			SendToUpateUI();
		
		}

		//���˲�������Ч��
		if (getActionIDByActionType(m_nowAction) == 9)
		{
			m_hurtEffect->setPosition(m_armature->getPosition());
			m_hurtEffect->resetSystem();
		}

		//����ֹͣ��ʱ��
		if (getActionIDByActionType(m_nowAction) == 11)
		{
			log("player:%d failed!", m_playerID);
			m_armature->setPositionY(m_armature->getPositionY() - 32);
			NotificationCenter::getInstance()->postNotification("resetScene", nullptr);

			//test
			//Director::getInstance()->pause();
			//auto delay = DelayTime::create(1.0F);
			//auto sendreset = CallFunc::create([&]() {
			//	NotificationCenter::getInstance()->postNotification("resetScene", nullptr);
			//});
			//this->runAction(Sequence::create(delay, sendreset, NULL));

			return;
		}

		log("Player: %d,Health: %d,Power: %d", m_playerID, m_nowHealth, m_nowPower);

		ReLoadAction();
	}
}
