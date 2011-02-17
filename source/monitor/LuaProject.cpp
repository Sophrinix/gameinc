#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaProject.h"
#include "INrpProject.h"
#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_LUAPROJECT( "CLuaProject" );

BEGIN_LUNA_METHODS(CLuaProject)
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaProject)
	LUNA_ILUAPROJECT_PROPERTIES( CLuaProject )
END_LUNA_PROPERTIES

CLuaProject::CLuaProject(lua_State *L, bool ex)	: ILuaProject(L, CLASS_LUAPROJECT, ex)							//конструктор
{}

const char* CLuaProject::ClassName()
{
	return ( CLASS_LUAPROJECT );
}
}//namespace nrp