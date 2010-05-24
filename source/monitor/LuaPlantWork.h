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
	static const char* StaticGetLuaName() { return CLASS_LUAPLANTWORK.c_str(); }

	CLuaPlantWork(lua_State *L);
	int Create( lua_State* L );
	int Remove( lua_State* L );
	int SetProduceType( lua_State* L );
	int SetGame( lua_State* L );
	int GetRentPrice( lua_State* L );
	int GetHourPerfomance( lua_State* L );
	int GetHourPrice( lua_State* L );
	int GetNumberMachine( lua_State* L );
	int GetNumberDay( lua_State* L );
	int GetNumberDisk( lua_State* L );
	int SetNumberMachine( lua_State* L );
	int SetNumberDay( lua_State* L );
	int GetDiskPrice( lua_State* L );
	int GetPrice( lua_State* L );
	int GetAdvPrice( lua_State* L );
	int GetDiskInDay( lua_State* L );
};

}//namespace nrp