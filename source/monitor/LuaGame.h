#pragma once

#include "ILuaObject.h"

namespace nrp
{

class CNrpGame;

class CLuaGame : public ILuaObject<nrp::CNrpGame>
{
public:
	static Luna<CLuaGame>::RegType methods[];				//методы обертки

	CLuaGame(lua_State *L);		
	int HaveBox( lua_State* L );
	int GetName( lua_State* L );
	int IsMyBoxAddon( lua_State* L );
	int RemoveBoxAddon( lua_State* L );
	int AddBoxAddon( lua_State* L );
	int GetBoxAddonsNumber( lua_State* L );
	int GetBoxAddon( lua_State* L );
	int CreateBox( lua_State* L );
	int RemoveBox( lua_State* L );
	int GetBoxLevel( lua_State* L );
	int IsSaling( lua_State* L );
															//нажатии на эту кнопку
	static const char* StaticGetLuaName() { return "CLuaGame"; }
};

}//namespace nrp