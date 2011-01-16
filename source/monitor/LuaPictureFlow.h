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
	DEFINE_PROPERTIES_AND_METHODS(CLuaPictureFlow)
	static const char* ClassName();

	CLuaPictureFlow(lua_State *L, bool);		
	
	int AddItem( lua_State *L );
	int GetSelected( lua_State *L );
	int GetSelectedItem( lua_State* L );
	int GetSelectedObject( lua_State* L );
	int SetSelected( lua_State *L );
	int SetPictureRect( lua_State* L );
	int SetDrawBorder( lua_State* L );
	int SetItemTexture( lua_State* L );
	int SetItemBlend( lua_State* L );
	int Clear( lua_State* L );
};

}//namespace nrp