#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class IGUITable;
	}
}

namespace nrp
{
CLASS_NAME CLASS_LUATABLE( "CLuaTable" );

class CLuaTable : public ILuaGuiElement<irr::gui::IGUITable>
{
public:
	static Luna<CLuaTable>::RegType methods[];				//методы обертки

	CLuaTable(lua_State *L);									
	int SetImage( lua_State *L );							//привязка к текстуре в обычном состоянии
	int SetAction( lua_State *L );							//установка имени функции луа, которая будет вызвана при 
	int AddRow( lua_State *L );
	int AddColumn( lua_State *L );
	int SetColumnWidth( lua_State *L );
	int SetCellText( lua_State *L );
	int SetRowHeight( lua_State *L );
	int GetRowCount( lua_State *L );
	int GetColumnCount( lua_State *L );
	int RemoveColumn( lua_State *L );
	int ClearRows( lua_State* L);

	static const char* ClassName() { return CLASS_LUATABLE.c_str(); }
};

}//namespace nrp