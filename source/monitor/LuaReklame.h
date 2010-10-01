#pragma once

#include "ILuaProject.h"

const std::string CLASS_LUAREKLAME( "CLuaReklame" );

namespace nrp
{
	class CNrpReklameWork;
}

namespace nrp
{

class CLuaReklame : public ILuaProject<nrp::CNrpReklameWork>
{
public:
	static Luna<CLuaReklame>::RegType methods[];				//методы обертки

	CLuaReklame(lua_State *L);		
	int GetQuality( lua_State* L );
	int GetLevel( lua_State* L );
	int Load( lua_State* L );
	int GetTypeName( lua_State* L );
	int GetNumberDay( lua_State* L );
	int SetNumberDay( lua_State* L );
	int GetPrice( lua_State* L );
	int GetDayCost( lua_State* L );
	int Remove( lua_State* L );
	int GetTexture( lua_State* L );
	int Create( lua_State* L );
	int GetFamous( lua_State* L );
	int SetReklameObject( lua_State* L );

	static const char* StaticGetLuaName() { return CLASS_LUAREKLAME.c_str(); }	
};

}//namespace nrp