#pragma once

#include "ILuaBaseProject.h"

namespace nrp
{
class CNrpPlant;

class CLuaPlant : public ILuaBaseProject<CNrpPlant>
{
public:
	static Luna<CLuaPlant>::RegType methods[];				//методы обертки

	CLuaPlant(lua_State *L);		
	int Load( lua_State* L );
	int GetBaseReklameNumber( lua_State* L );
	int GetBaseReklame( lua_State* L );
	int LoadBaseReklame( lua_State* L );
	int LoadReklamePrice( lua_State* L );
	int SaveReklamePrice( lua_State* L );
	int AddReklameWork( lua_State* L );
	int Save( lua_State* L );
	int AddProduceWork( lua_State* L );
	int GetReklame( lua_State* L );
	int LoadDiskMachine( lua_State* L );
	int GetDiskMachineNumber( lua_State* L );
	int GetDiskMachine( lua_State* L );
																//нажатии на эту кнопку
	static const char* ClassName();
};

}//namespace nrp