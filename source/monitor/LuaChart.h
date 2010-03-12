#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CChartCtrl;
	}
}

namespace nrp
{

class CLuaChart : public ILuaGuiElement<irr::gui::CChartCtrl>
{
public:
	static Luna<CLuaChart>::RegType methods[];				//методы обертки

	CLuaChart(lua_State *L);		

	int SetAxisAutomatic( lua_State *L); 
	int Update( lua_State *L); 
	int AddSerie( lua_State *L); 
	int AddPoint( lua_State *L); 
	int UpdateData( lua_State *L);
	int SetOffsetPoints( lua_State *L);

	static const char* StaticGetLuaName() { return "CLuaChart"; }
};

}//namespace nrp