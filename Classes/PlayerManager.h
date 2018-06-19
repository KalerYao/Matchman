/*
	��ҹ�����
	������ҽ�ɫ�����߼�
*/

#pragma once
#include "Player.h"
#include "cocos2d.h"
#include <map>
#include <vector>

USING_NS_CC;

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();
	Player *getPlayerByID(int i);

	//��Player�ӵ�������
	void AddPlayerToScene(Scene* sceneToAdd);

	//��ȡ�����Ͷ����Ķ�Ӧ��ϵ
	static void LoadActionKeyAndIDFromLua(const char* file);
	static std::vector<std::vector<EventKeyboard::KeyCode>> getAllKey();
	//����KeyCodeִ�����Action
	void DoActionByKeyCode(int playerID, EventKeyboard::KeyCode keyCode);
private:
	//���
	std::vector<Player*> m_players;
	//��¼�����Ͷ���ID
	static std::map<EventKeyboard::KeyCode, int> m_actionKeyAndID;
	//���ݰ����ַ���ת�ɼ��̷�
	static EventKeyboard::KeyCode getKeyCodeByKeyString(std::string key);
	//��¼�Ѿ��еİ���
	static std::vector<std::vector<EventKeyboard::KeyCode>> m_allKey;
	void TurnActionByKeyCode(Ref* pSender);
};