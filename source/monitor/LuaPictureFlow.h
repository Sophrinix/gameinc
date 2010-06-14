#pragma once

#include "ILuaGuiElement.h"

const std::string CLASS_LUAPICTUREFLOW( "CLuaPictureFlow" );

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

	CLuaPictureFlow(lua_State *L);		
	
	int AddItem( lua_State *L );
	int GetSelected( lua_State *L );
	int GetSelectedItem( lua_State* L );
	int GetSelectedObject( lua_State* L );
	int SetSelected( lua_State *L );
	int SetPictureRect( lua_State* L );
	int Clear( lua_State* L );

	static const char* StaticGetLuaName() { return CLASS_LUAPICTUREFLOW.c_str(); }
};

}//namespace nrp