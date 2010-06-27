#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpProjectModule;
}

namespace nrp
{

const std::string CLASS_DEVELOPMODULE( "CLuaDevelopModule" );

class CLuaDevelopModule : public ILuaProject<nrp::CNrpProjectModule>
{
public:
	static Luna<CLuaDevelopModule>::RegType methods[];				//методы обертки

	CLuaDevelopModule(lua_State *L);		
	int GetLevel( lua_State* L );
	int GetOptionAsInt( lua_State* L );
	int GetTechGroup( lua_State* L );
	int HaveLider( lua_State* L );
	int GetEmployerPosibility( lua_State* L );
	int Remove( lua_State* L );
	int GetTexture( lua_State* L );

	static const char* StaticGetLuaName() { return CLASS_DEVELOPMODULE.c_str(); }	
};

}//namespace nrp