#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaProject.h"
#include "INrpProject.h"
#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_LUAPROJECT( "CLuaProject" );

Luna< CLuaProject >::RegType CLuaProject::methods[] =			//���������� ������
{
	LUNA_ILUAPROJECT_HEADER( CLuaProject ),
	/*   */
	{0,0}
};

CLuaProject::CLuaProject(lua_State *L)	: ILuaProject(L, "CLuaProject")							//�����������
{}

const char* CLuaProject::ClassName()
{
	return ( CLASS_LUAPROJECT );
}
}//namespace nrp