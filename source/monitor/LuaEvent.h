#pragma once
#include "ILuaObject.h"


namespace irr
{
	struct SEvent;
}

namespace nrp
{
CLASS_NAME CLASS_LUAEVENT( "CLuaEvent" );

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

	static const char* ClassName() { return CLASS_LUAEVENT.c_str(); }
};

}//namespace nrp