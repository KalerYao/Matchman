#include "LuaUtil.h"

lua_State* LuaUtil::openLuaFile(std::string file)
{
	//ִ��lua�ű���ȡ
	lua_State* pL = lua_open();
	luaopen_base(pL);
	luaopen_math(pL);
	luaopen_string(pL);

	//ִ��lua�ű�������0����ɹ�
	int err = luaL_dofile(pL, file.c_str());
	log("open = %d", err);
	//����ջ������
	lua_settop(pL, 0);

	return pL;
}

void LuaUtil::closeLuaFile(lua_State* pL)
{
	lua_close(pL);
}
