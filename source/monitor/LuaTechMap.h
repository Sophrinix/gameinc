#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpTechMap;
	}
}

namespace nrp
{
class CLuaTechMap : public ILuaGuiElement<irr::gui::CNrpTechMap>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaTechMap)
	static const char* ClassName();

	CLuaTechMap(lua_State *L, bool);									
	int SetImage( lua_State *L );							//привязка к текстуре в обычном состоянии
	int SetAction( lua_State *L );							//установка имени функции луа, которая будет вызвана при 
	int AddLuaFunction( lua_State* L );
	int AddTechnology( lua_State* L );
	int IsTechHaveUndiscoverChild( lua_State* L );
	int GetSelectedObjectType( lua_State* L );
	int GetSelectedObject( lua_State* L );
	int GetSelectedObjectName( lua_State* L );
	int SetDrawBack( lua_State* L );
	int SetOnActivate( lua_State* L );
};

}//namespace nrp