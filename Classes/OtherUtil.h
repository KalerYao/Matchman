#pragma once
#include <vector>
#include <algorithm>
#include <iterator>

static class OtherUtil
{
public:
	//vector���Ƿ������Ԫ��
	template <class T>
	static bool isContain(std::vector<T> vec, T e);
};

template <typename T>
bool OtherUtil::isContain(std::vector<T> vec, T e)
{
	std::vector<T>::iterator ite = find(vec.begin(), vec.end(), e);
	if (ite != vec.end())
		return true;
	return false;
}

//UI��Ϣ
class UIMessage : public Ref
{
public:
	int id;
	int health;
	int power;
};