#pragma once

#include "ILuaObject.h"


namespace nrp
{

const std::string CLASS_LUADISKMACHINE("CLuaDiskMachine");

class CNrpDiskMachine;

class CLuaDiskMachine : public ILuaObject< nrp::CNrpDiskMachine >
{
public:
	static Luna<CLuaDiskMachine>::RegType methods[];

	CLuaDiskMachine(lua_State *L);
	int Create( lua_State* L );
	int Remove( lua_State* L );
	int Load( lua_State* L );
	int IsLoaded( lua_State* L );
	int GetName( lua_State* L );

	static const char* StaticGetLuaName() { return CLASS_LUADISKMACHINE.c_str(); }
};

}//namespace nrp