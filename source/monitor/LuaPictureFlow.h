#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrp2DPictureFlow;
	}
}

namespace nrp
{

class CLuaPictureFlow : public ILuaGuiElement<irr::gui::CNrp2DPictureFlow>
{
public:
	static Luna<CLuaPictureFlow>::RegType methods[];				//методы обертки
	static const char* ClassName();

	CLuaPictureFlow(lua_State *L);		
	
	int AddItem( lua_State *L );
	int GetSelected( lua_State *L );
	int GetSelectedItem( lua_State* L );
	int GetSelectedObject( lua_State* L );
	int SetSelected( lua_State *L );
	int SetPictureRect( lua_State* L );
	int SetDrawBorder( lua_State* L );
	int Clear( lua_State* L );
};

}//namespace nrp