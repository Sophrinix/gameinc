#ifndef _INCLUDE_LUAGUILINK_
#define _INCLUDE_LUAGUILINK_
 
#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpGuiLink;
	}
}

namespace nrp
{

class CLuaGuiLink : public ILuaGuiElement<irr::gui::CNrpGuiLink>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaGuiLink)				//методы обертки

	CLuaGuiLink(lua_State *L, bool);		
	int SetAction( lua_State *L );		  					//установка имени функции луа, которая будет вызвана при
	int SetTooltip( lua_State* L );
	int SetOverrideColor( lua_State* L );
	static const char* ClassName();
};

}//namespace nrp

#endif