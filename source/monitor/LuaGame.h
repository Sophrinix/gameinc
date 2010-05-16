#pragma once

#include "ILuaObject.h"

namespace nrp
{

class CNrpGame;

class CLuaGame : public ILuaObject<nrp::CNrpGame>
{
public:
	static Luna<CLuaGame>::RegType methods[];				//������ �������

	CLuaGame(lua_State *L);		
	int HaveBox( lua_State* L );
	int GetName( lua_State* L );
	int IsMyBoxAddon( lua_State* L );
	int RemoveBoxAddon( lua_State* L );
	int AddBoxAddon( lua_State* L );
															//������� �� ��� ������
	static const char* StaticGetLuaName() { return "CLuaGame"; }
};

}//namespace nrp