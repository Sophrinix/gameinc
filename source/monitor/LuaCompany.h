#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpCompany;
}

namespace nrp
{

class CLuaCompany : public ILuaProject<nrp::CNrpCompany>
{
public:
	static Luna<CLuaCompany>::RegType methods[];				//������ �������

	CLuaCompany(lua_State *L);		
	int SetCEO( lua_State* L );
	int GetName( lua_State* L );
	int GetBalance( lua_State* L );
	int GetEnginesNumber( lua_State* L );
	int GetEngine( lua_State* L );
	int AddGameEngine( lua_State* L );
	int GetTechNumber( lua_State* L );
	int GetTech( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaCompany"; }
};

}//namespace nrp