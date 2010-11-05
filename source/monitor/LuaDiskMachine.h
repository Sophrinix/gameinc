#pragma once

#include "ILuaBaseProject.h"


namespace nrp
{

CLASS_NAME CLASS_LUADISKMACHINE("CLuaDiskMachine");

class CNrpDiskMachine;

class CLuaDiskMachine : public ILuaBaseProject< nrp::CNrpDiskMachine >
{
public:
	static Luna<CLuaDiskMachine>::RegType methods[];

	CLuaDiskMachine(lua_State *L);
	int Create( lua_State* L );
	int Remove( lua_State* L );
	int Load( lua_State* L );
	int IsLoaded( lua_State* L );
	int GetName( lua_State* L );
	int GetTexture( lua_State* L );

	static const char* ClassName() { return CLASS_LUADISKMACHINE.c_str(); }
};

}//namespace nrp