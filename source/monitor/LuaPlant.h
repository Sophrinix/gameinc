#pragma once

#include "ILuaBaseProject.h"

namespace nrp
{
class CNrpPlant;

class CLuaPlant : public ILuaBaseProject<CNrpPlant>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaPlant)

	CLuaPlant(lua_State *L, bool);		
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
	int GetCampaniesNumber( lua_State* L );
    int GetWorksNumber( lua_State* L );
    int GetProduceWork( lua_State* L );
															
	static const char* ClassName();
};

}//namespace nrp