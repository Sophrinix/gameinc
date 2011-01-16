#pragma once
#include "ILuaGuiElement.h"

namespace nrp
{
class CNrpPda;

class CLuaPda : public ILuaObject<CNrpPda>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaPda)
	static const char* ClassName();

	CLuaPda(lua_State *L, bool );
	int GetMessage( lua_State* L );
	int GetTimeStr( lua_State* L );
	int Next( lua_State* L );
	int Prev( lua_State* L );
	int AddMessage( lua_State* L );
	int Save( lua_State* L );
	int Load( lua_State* L );
};

}//namespace nrp