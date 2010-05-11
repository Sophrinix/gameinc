#pragma once

#include "ILuaObject.h"

namespace nrp
{

class CNrpGameBox;

class CLuaGameBox : public ILuaObject<nrp::CNrpGameBox>
{
public:
	static Luna<CLuaGameBox>::RegType methods[];				//методы обертки

	CLuaGameBox(lua_State *L);		
																//нажатии на эту кнопку
	static const char* StaticGetLuaName() { return "CLuaGameBox"; }
};

}//namespace nrp