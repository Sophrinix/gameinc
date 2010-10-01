#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class INrpDevelopProject;
}

namespace nrp
{

const std::string CLASS_LUADEVPROJECT( "CLuaDevelopProject" );

class CLuaDevelopProject : public ILuaProject<nrp::INrpDevelopProject>
{
public:
	static Luna<CLuaDevelopProject>::RegType methods[];				//методы обертки

	CLuaDevelopProject(lua_State *L);	
	int GetModuleNumber( lua_State* L );
	int GetModule( lua_State* L );
	int GetFamous( lua_State* L );

	static const char* StaticGetLuaName() { return CLASS_LUADEVPROJECT.c_str(); }
};

}//namespace nrp