#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpTechnology;
}

namespace nrp
{

class CLuaTechnology : public ILuaProject<nrp::CNrpTechnology>
{
public:
	static Luna<CLuaTechnology>::RegType methods[];				//методы обертки

	CLuaTechnology(lua_State *L);		
	int SetTechType( lua_State* L );
	int SetName( lua_State* L );
	int GetName( lua_State* L );
	int SetBaseCode( lua_State* L );
	int SetAddingEngineCode( lua_State* L );
	int SetEngineTechRequire( lua_State* L );
	int SetEmployerSkillRequire( lua_State* L );
	int GetTechType( lua_State* L );
	int GetOptionAsInt( lua_State* L );
	int GetTechGroup( lua_State* L );
																//нажатии на эту кнопку
	static const char* StaticGetLuaName() { return "CLuaTech"; }
};

}//namespace nrp