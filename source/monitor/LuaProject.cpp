#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaProject.h"

namespace nrp
{

Luna< CLuaProject >::RegType CLuaProject::methods[] =			//реализуемы методы
{
	LUNA_ILUAPROJECT_HEADER( CLuaProject ),
	/*   */
	{0,0}
};

CLuaProject::CLuaProject(lua_State *L)	: ILuaProject(L, "CLuaProject")							//конструктор
{}

}//namespace nrp