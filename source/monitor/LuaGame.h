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
															//нажатии на эту кнопку
	static const char* StaticGetLuaName() { return "CLuaGame"; }
};

}//namespace nrp