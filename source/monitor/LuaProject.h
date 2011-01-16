#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class INrpProject;
}

namespace nrp
{

class CLuaProject : public ILuaProject<nrp::INrpProject>
{
public:
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaProject)
	static const char* ClassName();

	CLuaProject(lua_State *L, bool );	
};

}//namespace nrp