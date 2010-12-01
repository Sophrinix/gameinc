#pragma once
#include "ILuaProject.h"

namespace nrp
{

class CLuaPlatform : public ILuaProject<nrp::CNrpPlatform>
{
public:
	static Luna<CLuaPlatform>::RegType methods[];				//методы обертки
	static const char* ClassName();

	CLuaPlatform(lua_State *L);		

	int Load( lua_State* L );
	int Create( lua_State* L );
	int GetTexture( lua_State* L );
};

}//end namespace nrp