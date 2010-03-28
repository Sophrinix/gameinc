#include "StdAfx.h"
#include <string>
#include "LuaUser.h"
#include "StrConversation.h"

using namespace irr;

namespace nrp
{

Luna< CLuaUser >::RegType CLuaUser::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaUser ),
	/*  */
	{0,0}
};

CLuaUser::CLuaUser(lua_State *L) : ILuaObject(L, "CLuaUser") {}

}//namespace nrp