#pragma once

#include "ILuaProject.h"

namespace nrp
{
	class CNrpReklameWork;
}

namespace nrp
{

class CLuaReklame : public ILuaProject<nrp::CNrpReklameWork>
{
public:
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaReklame)
	static const char* ClassName();

	CLuaReklame(lua_State *L, bool );		
	int GetQuality( lua_State* L );
	int GetLevel( lua_State* L );
	int Load( lua_State* L );
	int GetNumberDay( lua_State* L );
	int SetNumberDay( lua_State* L );
	int GetPrice( lua_State* L );
	int GetDayCost( lua_State* L );
	int Remove( lua_State* L );
	int Create( lua_State* L );
	int GetFamous( lua_State* L );
	int SetReklameObject( lua_State* L );
	int SetCompanyName( lua_State* L );
	int GetCompanyName( lua_State* L );
	int GetObjectName( lua_State* L );
	int GetObjectRType( lua_State* L );
};

}//namespace nrp