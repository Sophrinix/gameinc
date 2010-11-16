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
	static Luna<CLuaProject>::RegType methods[];				//методы обертки
	static const char* ClassName();

	CLuaProject(lua_State *L);	
};

}//namespace nrp