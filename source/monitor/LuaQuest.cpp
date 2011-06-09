#include "StdAfx.h"
#include "LuaQuest.h"

namespace nrp
{
CLASS_NAME CLASS_CLUAQUEST( "CLuaQuest" );

BEGIN_LUNA_METHODS(CLuaQuest)
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaQuest)
	LUNA_ILUAPROJECT_PROPERTIES(CLuaQuest)
	LUNA_AUTONAME_PROPERTY( CLuaQuest, "x", GetX, SetX )
	LUNA_AUTONAME_PROPERTY( CLuaQuest, "y", GetY, SetY )
	//LUNA_AUTONAME_PROPERTY( CLuaQuest, "timeStart", GetTimeStart, SetTimeStart )
	LUNA_AUTONAME_PROPERTY( CLuaQuest, "start", PureFunction, SetFuncStart )
END_LUNA_PROPERTIES
	
CLuaQuest::CLuaQuest(lua_State *L, bool ex) : ILuaProject(L, CLASS_CLUAQUEST, ex )
{
}

CLuaQuest::~CLuaQuest(void)
{
}

const char* CLuaQuest::ClassName()
{
	return CLASS_CLUAQUEST;
}

int CLuaQuest::GetX( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN { lua_pushinteger( L, (*_object)[ POSX ] ); return 1; }

	lua_pushnil( L );
	return 1;
}

int CLuaQuest::SetX( lua_State* L )
{
	return SetParam_<int, lua_Integer>( L, PROP, POSX, lua_tointeger );
}

int CLuaQuest::GetY( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN { lua_pushinteger( L, (*_object)[ POSY ] ); return 1; }

	lua_pushnil( L );
	return 1;
}

int CLuaQuest::SetY( lua_State* L )
{
	return SetParam_<int, lua_Integer>( L, PROP, POSY, lua_tointeger );
}

int CLuaQuest::SetFuncStart( lua_State* L )
{
	assert( lua_isfunction( L, -1 ) && "param for CLuaQuest.start must be function" );
	IF_OBJECT_NOT_NULL_THEN	
		(*_object)[ STARTFUNCREF ] = _GetRef( L, -1 );
	return 0;
}
}