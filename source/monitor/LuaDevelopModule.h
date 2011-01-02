#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpProjectModule;
}

namespace nrp
{

class CLuaDevelopModule : public ILuaProject<nrp::CNrpProjectModule>
{
public:
	static Luna<CLuaDevelopModule>::RegType methods[];				//методы обертки
	static const char* ClassName();

	CLuaDevelopModule(lua_State *L);		
	int GetLevel( lua_State* L );
	int GetOption( lua_State* L );
	int GetPercentDone( lua_State* L );
	int GetQuality( lua_State* L );
	int GetEmployerPosibility( lua_State* L );
	int Remove( lua_State* L );
	int GetTexture( lua_State* L );
	int GetParent( lua_State* L );
};

}//namespace nrp