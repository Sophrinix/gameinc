#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class HTMLEngine;
}

namespace nrp
{

class CLuaBrowser : public ILuaProject<nrp::HTMLEngine>
{
public:
	static Luna<CLuaBrowser>::RegType methods[];				//методы обертки

	CLuaBrowser(lua_State *L);		
	int Show( lua_State* L );
	int Hide( lua_State* L );
	int Navigate( lua_State* L );
	int GetWindow( lua_State *L );
	int Move( lua_State *L );

	static const char* ClassName();
};

}//namespace nrp