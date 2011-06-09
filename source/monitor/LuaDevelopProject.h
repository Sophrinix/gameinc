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
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaDevelopProject)
	static const char* ClassName();

	CLuaDevelopProject(lua_State *L, bool);	
	int GetModuleNumber( lua_State* L );
	int GetModule( lua_State* L );
	int GetFamous( lua_State* L );
	int GetWorkPercentDone( lua_State* L );
	int GetCodeVolume( lua_State* L );
};

}//namespace nrp