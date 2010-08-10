#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpInvention;
}

namespace nrp
{

class CLuaInvention : public ILuaProject<nrp::CNrpInvention>
{
public:
	static Luna<CLuaInvention>::RegType methods[];				//методы обертки

	CLuaInvention(lua_State *L);		
	int AddUser( lua_State* L );
	int Load( lua_State* L );
	int Remove( lua_State* L );
	int Create( lua_State* L );
	int GetTexture( lua_State* L );
	int HaveRequireTech( lua_State* L );
	int GetFutureTechNumber( lua_State* L );
	int GetLevel( lua_State* L );
	int GetStatus( lua_State* L );
	int GetDescriptionLink( lua_State* L );
	int GetInvestiment( lua_State* L );
	int SetInvestiment( lua_State* L );
	int GetSpeed( lua_State* L );
	int GetPrognoseDateFinish( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaInvention"; }	
};

}//namespace nrp