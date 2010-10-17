#pragma once
#include "NrpRelation.h"
#include "ILuaObject.h"

namespace nrp
{
CLASS_NAME CLASS_RELATION( "CLuaRelation" );

class CLuaRelation : public ILuaObject<nrp::CNrpRelation>
{
public:
	static Luna<CLuaRelation>::RegType methods[];

	CLuaRelation(lua_State *L);
	int SetValue( lua_State *L );
	int GetValue( lua_State *L );

	static const char* ClassName() { return CLASS_RELATION.c_str(); }
	virtual std::string ObjectName() { return CLASS_RELATION; }
};

}//namespace nrp