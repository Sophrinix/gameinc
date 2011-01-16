#pragma once
#include "NrpRelation.h"
#include "ILuaObject.h"

namespace nrp
{

class CLuaRelation : public ILuaObject<nrp::CNrpRelation>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaRelation)
	static const char* ClassName();

	CLuaRelation(lua_State *L, bool);
	int Set( lua_State *L );
	int Get( lua_State *L );
};

}//namespace nrp