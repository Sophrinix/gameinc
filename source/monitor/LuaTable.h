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
class CLuaTable : public ILuaGuiElement<irr::gui::IGUITable>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaTable)
	static const char* ClassName();

	CLuaTable(lua_State *L, bool);									
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
	int GetActiveColumn( lua_State* L );
	int SetCellSelected( lua_State* L );
};

}//namespace nrp