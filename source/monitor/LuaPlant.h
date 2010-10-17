#pragma once

#include "ILuaObject.h"

namespace nrp
{

CLASS_NAME CLASS_LUAPLANT( "CLuaPlant" );
class CNrpPlant;

class CLuaPlant : public ILuaObject<CNrpPlant>
{
public:
	static Luna<CLuaPlant>::RegType methods[];				//методы обертки

	CLuaPlant(lua_State *L);		
	int Load( lua_State* L );
	int LoadBaseReklame( lua_State* L );
	int LoadReklamePrice( lua_State* L );
	int SaveReklamePrice( lua_State* L );
	int AddReklameWork( lua_State* L );
	int Save( lua_State* L );
	int AddProduceWork( lua_State* L );
	int GetReklame( lua_State* L );
																//нажатии на эту кнопку
	static const char* ClassName() { return CLASS_LUAPLANT.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUAPLANT; }
};

}//namespace nrp