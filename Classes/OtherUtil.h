#pragma once
#include <vector>


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
	for (T v : vec)
	{
		if (v == e)
			return true;
	}
	return false;
}