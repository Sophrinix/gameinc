#pragma once

#include "ILuaBaseProject.h"


namespace nrp
{

class CNrpDiskMachine;

class CLuaDiskMachine : public ILuaBaseProject< nrp::CNrpDiskMachine >
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaDiskMachine)
	static const char* ClassName();

	CLuaDiskMachine(lua_State *L, bool );
	int Create( lua_State* L );
	int Remove( lua_State* L );
	int Load( lua_State* L );
	int IsLoaded( lua_State* L );
	int GetName( lua_State* L );
	int GetTexture( lua_State* L );
	int ChangeDiscount( lua_State* L );
	int GetDiscount( lua_State* L );
	int SetDiscount( lua_State* L );
	int GetMaxDiscount( lua_State* L );
	int GetDiskProduced( lua_State* L );
	int GetLineDiscount( lua_State* L );
};

}//namespace nrp