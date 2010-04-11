#pragma once
#include "ILuaObject.h"


namespace irr
{
	struct SEvent;
}

namespace nrp
{

class CLuaEvent : public ILuaObject<irr::SEvent>
{
public:
	static Luna<CLuaEvent>::RegType methods[];

	CLuaEvent(lua_State *L);
	int GetUserData1( lua_State* L );
	int GetLogText( lua_State* L );
	int GetTime( lua_State* L );
	int GetChar( lua_State* L );
	int IsKeyDown( lua_State* L );
	int GetGuiCaller( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaEvent"; }
};

}//namespace nrp