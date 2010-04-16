#pragma once
#include "ILuaObject.h"

namespace nrp
{
	class IUser;
}

namespace nrp
{

class CLuaUser : public ILuaObject<nrp::IUser>
{
public:
	static Luna<CLuaUser>::RegType methods[];

	CLuaUser(lua_State *L);

	int SetSkill( lua_State* L );
	int GetSkill( lua_State* L );
	int SetCharacter( lua_State* L );
	int GetTypeName( lua_State* L );
	int GetParam( lua_State* L );
	int SetParam( lua_State* L );
	int GetName( lua_State* L );
	int IsTypeAs( lua_State* L );
	int Save( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaUser"; }
};

}//namespace nrp