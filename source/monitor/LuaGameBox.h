#pragma once

#include "ILuaObject.h"

namespace nrp
{
class CNrpGameBox;

class CLuaGameBox : public ILuaObject<nrp::CNrpGameBox>
{
public:
	static Luna<CLuaGameBox>::RegType methods[];				//методы обертки
	static const char* ClassName();

	CLuaGameBox(lua_State *L);		
};

}//namespace nrp