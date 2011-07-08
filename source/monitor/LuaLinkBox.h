#pragma once

#include "ILuaGuiElement.h"
#include "nrpGuiLinkBox.h"

namespace nrp
{

class CLuaLinkBox : public ILuaGuiElement<irr::gui::CNrpGuiLinkBox>
{
public:
    static const NrpText rta_top;
    static const NrpText rta_left;
    static const NrpText rta_bottom;
    static const NrpText rta_right;
    static const NrpText rta_center;
	DEFINE_PROPERTIES_AND_METHODS(CLuaLinkBox)

	static const char* ClassName();
	CLuaLinkBox(lua_State *L, bool);		
																//нажатии на эту кнопку
	int Bind( lua_State* L );
	int Unbind( lua_State* L );
	int SetModuleType( lua_State* L );
	int GetModuleType( lua_State* L );
	int SetDraggable( lua_State* L );
	int IsDraggable( lua_State* L );
	int SetTexture( lua_State* L );
	int GetTexture( lua_State* L );
	int SetDefaultTexture( lua_State* L );
	int SetData( lua_State* L );
	int GetData( lua_State* L );
	int HaveData( lua_State* L );
	int SetFunction( lua_State* L );
	int UnsetFunction( lua_State* L );
	int SetColor( lua_State* L );
    int SetTextPos( lua_State* L );
    int SetDragStartFunction( lua_State* L );

private:
	typedef enum { TI_IMAGE=0, TI_HOVER, TI_PRESSED } TYPE_IMAGE;
	int AddRemLuaFunction_( lua_State* L, const NrpText& funcName, bool add );
	
};

}//namespace nrp