#pragma once

#include "ILuaObject.h"

namespace nrp
{
class CNrpPlantWork;

class CLuaPlantWork : public ILuaObject< nrp::CNrpPlantWork >
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaPlantWork)
	static const char* ClassName();

	CLuaPlantWork(lua_State *L, bool );
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