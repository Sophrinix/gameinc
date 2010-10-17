#pragma once

#include "ILuaObject.h"

namespace nrp
{
CLASS_NAME CLASS_LUAGAMEBOX( "CLuaGameBox" );

class CNrpGameBox;

class CLuaGameBox : public ILuaObject<nrp::CNrpGameBox>
{
public:
	static Luna<CLuaGameBox>::RegType methods[];				//методы обертки

	CLuaGameBox(lua_State *L);		
																//нажатии на эту кнопку
	static const char* ClassName() { return CLASS_LUAGAMEBOX.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUAGAMEBOX; }
};

}//namespace nrp