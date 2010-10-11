#pragma once
#include "NrpRelation.h"
#include "ILuaObject.h"

namespace nrp
{

class CLuaRelation : public ILuaObject<nrp::CNrpRelation>
{
public:
	static Luna<CLuaRelation>::RegType methods[];

	CLuaRelation(lua_State *L);
	int SetValue( lua_State *L );
	int GetValue( lua_State *L );

	static const char* StaticGetLuaName() { return "CLuaRelation"; }
};

}//namespace nrp