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
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaBrowser)

	CLuaBrowser(lua_State *L, bool );		
	int Show( lua_State* L );
	int Hide( lua_State* L );
	int Navigate( lua_State* L );
	int GetWindow( lua_State *L );
	int Move( lua_State *L );
	int SetPage404( lua_State* L );

	static const char* ClassName();
};

}//namespace nrp