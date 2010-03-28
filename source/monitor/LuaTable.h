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
	static Luna<CLuaTable>::RegType methods[];				//������ �������

	CLuaTable(lua_State *L);									
	int SetImage( lua_State *L );							//�������� � �������� � ������� ���������
	int SetAction( lua_State *L );							//��������� ����� ������� ���, ������� ����� ������� ��� 
	int AddRow( lua_State *L );
	int AddColumn( lua_State *L );
	int SetColumnWidth( lua_State *L );
	int SetCellText( lua_State *L );
	int SetRowHeight( lua_State *L );
	int GetRowCount( lua_State *L );
	int GetColumnCount( lua_State *L );
	int RemoveColumn( lua_State *L );
	int ClearRows( lua_State* L);

	static const char* StaticGetLuaName() { return "CLuaTable"; }
};

}//namespace nrp