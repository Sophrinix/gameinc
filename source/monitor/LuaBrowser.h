#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class HTMLEngine;
}

namespace nrp
{

const std::string CLASS_LUABROWSER( "CLuaBrowser" );

class CLuaBrowser : public ILuaProject<nrp::HTMLEngine>
{
public:
	static Luna<CLuaBrowser>::RegType methods[];				//методы обертки

	CLuaBrowser(lua_State *L);		
	int Show( lua_State* L );
	int Navigate( lua_State* L );


	static const char* StaticGetLuaName() { return CLASS_LUABROWSER.c_str(); }
};

}//namespace nrp