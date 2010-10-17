#pragma once

#include "ILuaProject.h"
#include "NrpGameEngine.h"

namespace nrp
{
CLASS_NAME CLASS_LUAGAMEENGINE( "CLuaGameEngine" );

class CLuaGameEngine : public ILuaProject<nrp::CNrpGameEngine>
{
public:
	static Luna<CLuaGameEngine>::RegType methods[];				//методы обертки

	CLuaGameEngine(lua_State *L);		
	int AddGenre( lua_State* L );
	int SetGenreModuleNumber( lua_State* L );
	int GetGenreModuleNumber( lua_State* L );
	int GetAdvancedTechNumber( lua_State* L );
	int SetCodeVolume( lua_State *L );
	int GetName( lua_State* L );
	int Create( lua_State* L );
	int GetTexture( lua_State* L );
	int Load( lua_State* L );

	static const char* ClassName() { return CLASS_LUAGAMEENGINE.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUAGAMEENGINE; }
};

}//namespace nrp