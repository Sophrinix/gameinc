#pragma once

#include "ILuaProject.h"
#include "NrpGameEngine.h"

namespace nrp
{

class CLuaGameEngine : public ILuaProject<nrp::CNrpGameEngine>
{
public:
	static Luna<CLuaGameEngine>::RegType methods[];				//методы обертки
	static const char* ClassName();

	CLuaGameEngine(lua_State *L);		
	int AddGenre( lua_State* L );
	int SetGenreModuleNumber( lua_State* L );
	int GetGenreModuleNumber( lua_State* L );
	int GetTechNumber( lua_State* L );
	int IsMyTech( lua_State* L );
	int SetCodeVolume( lua_State *L );
	int Create( lua_State* L );
	int GetTexture( lua_State* L );
	int Load( lua_State* L );
};

}//namespace nrp