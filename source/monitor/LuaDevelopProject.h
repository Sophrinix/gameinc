#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class INrpDevelopProject;
}

namespace nrp
{
class CLuaDevelopProject : public ILuaProject<nrp::INrpDevelopProject>
{
public:
	static Luna<CLuaDevelopProject>::RegType methods[];				//методы обертки
	static const char* ClassName();

	CLuaDevelopProject(lua_State *L);	
	int GetModuleNumber( lua_State* L );
	int GetModule( lua_State* L );
	int GetFamous( lua_State* L );
	int GetWorkPercentDone( lua_State* L );
};

}//namespace nrp