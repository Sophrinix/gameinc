#pragma once
#include "ILuaObject.h"

namespace nrp
{
	struct IUser;
}

namespace nrp
{

class CLuaUser : public ILuaObject<nrp::IUser>
{
public:
	static Luna<CLuaUser>::RegType methods[];

	CLuaUser(lua_State *L);

	static const char* StaticGetLuaName() { return "CLuaUser"; }
};

}//namespace nrp