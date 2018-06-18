#pragma once
#include <iostream>
#include <vector>
#include "cocos2d.h"

USING_NS_CC;

static class PlayerType
{
public:
	//��lua�ļ���ȡ����
	static void LoadTypeFromLua();
	static int GetPlayerTypeByOrder(int order);
private:
	//�洢�������
	static std::vector<int> m_playerTypes;
};