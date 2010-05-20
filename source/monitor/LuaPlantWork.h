#pragma once

#include "ILuaObject.h"

namespace nrp
{

const std::string CLASS_LUAPLANTWORK("CLuaPlantWork");

class CNrpPlantWork;

class CLuaPlantWork : public ILuaObject< nrp::CNrpPlantWork >
{
public:
	static Luna<CLuaPlantWork>::RegType methods[];

	CLuaPlantWork(lua_State *L);
	int Create( lua_State* L );

	static const char* StaticGetLuaName() { return CLASS_LUAPLANTWORK.c_str(); }
};

}//namespace nrp