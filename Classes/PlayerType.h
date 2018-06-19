#pragma once
#include <vector>
#include "cocos2d.h"

USING_NS_CC;

static class PlayerType
{
public:
	//��lua�ļ���ȡ����
	static void LoadTypeFromLua(const char* file);
	static std::string GetPlayerTypeByOrder(int order);
private:
	//�洢�������
	static std::vector<std::string> m_playerTypes;
};