/*
	�����
	������Ҳ��ݽ�ɫ�߼�
*/

#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PlayerType.h"

USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;



//�����
class Player : public Sprite
{
public:
	Player();
	~Player();
	static Player* create(int id, std::string type);
	static void LoadPlayerActionFromLua(const char* file);

	//������ת��
	bool TryTurnTo(std::string newAction);

	static std::string getPlayerTypeByID(int actionID);

private:

	static std::vector<std::string> m_playerActionType;

	bool init(int id, std::string type);

	//�����Ϣ
	int m_playerID;
	std::string m_playerType;
	Armature *m_armature;
	std::string m_nowAction;
};