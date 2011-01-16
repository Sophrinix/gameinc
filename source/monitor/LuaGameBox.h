#pragma once

#include "ILuaObject.h"

namespace nrp
{
class CNrpGameBox;

class CLuaGameBox : public ILuaObject<nrp::CNrpGameBox>
{
public:
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaGameBox)
	static const char* ClassName();

	CLuaGameBox(lua_State *L, bool );		
};

}//namespace nrp