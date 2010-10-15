#pragma once
#include "ILuaGuiElement.h"

namespace nrp
{

class CNrpPda;

class CLuaPda : public ILuaObject<CNrpPda>
{
public:
	static Luna<CLuaPda>::RegType methods[];

	CLuaPda(lua_State *L);
	int GetMessage( lua_State* L );
	int GetTimeStr( lua_State* L );
	int Next( lua_State* L );
	int Prev( lua_State* L );
	int AddMessage( lua_State* L );
	int Save( lua_State* L );
	int Load( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaPda"; }
};

}//namespace nrp