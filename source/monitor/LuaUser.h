#pragma once
#include "ILuaObject.h"

namespace nrp
{
	class IUser;
}

namespace nrp
{
CLASS_NAME CLASS_LUAUSER( "CLuaUser" );

class CLuaUser : public ILuaObject<nrp::IUser>
{
public:
	static Luna<CLuaUser>::RegType methods[];

	CLuaUser(lua_State *L);

	int SetSkill( lua_State* L );
	int GetSkill( lua_State* L );
	int SetCharacter( lua_State* L );
	int GetTypeName( lua_State* L );
	int IsFreeUser( lua_State* L );
	int GetParam( lua_State* L );
	int SetParam( lua_State* L );
	int GetName( lua_State* L );
	int IsTypeAs( lua_State* L );
	int AddWork( lua_State* L );
	int GetWorkNumber( lua_State* L);
	int RemoveWork( lua_State* L );
	int HaveInvention( lua_State* L );
	int GetWork( lua_State* L );
	int Save( lua_State* L );
	int Create( lua_State* L );
	int GetTexture( lua_State* L );
	int GetRelation( lua_State* L );

	static const char* ClassName() { return CLASS_LUAUSER.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUAUSER; }
};

}//namespace nrp