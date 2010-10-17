#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class INrpProject;
}

namespace nrp
{
CLASS_NAME CLASS_LUAPROJECT( "CLuaProject" );

class CLuaProject : public ILuaProject<nrp::INrpProject>
{
public:
	static Luna<CLuaProject>::RegType methods[];				//методы обертки

	CLuaProject(lua_State *L);	
	static const char* ClassName() { return CLASS_LUAPROJECT.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUAPROJECT; }
};

}//namespace nrp