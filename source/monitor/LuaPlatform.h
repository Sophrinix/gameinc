#pragma once
#include "ILuaProject.h"

namespace nrp
{

class CLuaPlatform : public ILuaProject<nrp::CNrpPlatform>
{
public:
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaPlatform)
	static const char* ClassName();

	CLuaPlatform(lua_State *L, bool );		

	int Load( lua_State* L );
	int Create( lua_State* L );
	int GetTexture( lua_State* L );
	int GetCpu( lua_State* L );
	int GetRam( lua_State* L );
	int IsMyTech( lua_State* L );
};

}//end namespace nrp