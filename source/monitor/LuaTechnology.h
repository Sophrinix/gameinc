#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpTechnology;
}

namespace nrp
{
CLASS_NAME CLASS_LUATECH( "CLuaTechnology" );

class CLuaTechnology : public ILuaProject<nrp::CNrpTechnology>
{
public:
	static Luna<CLuaTechnology>::RegType methods[];				//методы обертки

	CLuaTechnology(lua_State *L);		
	int SetTechType( lua_State* L );
	int SetBaseCode( lua_State* L );
	int SetAddingEngineCode( lua_State* L );
	int SetEngineTechRequire( lua_State* L );
	int SetEmployerSkillRequire( lua_State* L );
	int SetQuality( lua_State* L );
	int GetLevel( lua_State* L );
	int GetOptionAsInt( lua_State* L );
	int GetTechGroup( lua_State* L );
	int HaveLider( lua_State* L );
	int Load( lua_State* L );
	int Remove( lua_State* L );
	int Create( lua_State* L );
	int IsLoaded( lua_State* L );
	int SetTexture( lua_State* L );
	int GetTexture( lua_State* L );
	int HaveRequireTech( lua_State* L );
	int GetFutureTechNumber( lua_State* L );
	int SetStatus( lua_State* L );
	int GetStatus( lua_State* L );
	int GetFutureTechInternalName( lua_State* L );
	int GetDescriptionLink( lua_State* L );
	int GetCompany( lua_State* L );
	int SetCompany( lua_State* L );
	int GetInternalName( lua_State* L );

	static const char* ClassName() { return CLASS_LUATECH.c_str(); }	
	virtual std::string ObjectName() { return CLASS_LUATECH; }
};

}//namespace nrp