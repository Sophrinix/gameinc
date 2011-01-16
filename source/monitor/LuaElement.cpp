#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaElement.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAELEMENT( "CLuaElement" );

BEGIN_LUNA_METHODS(CLuaElement)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaElement )
	/*  */
	//LUNA_AUTONAME_FUNCTION( CLuaElement, RemoveChilds ),
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaElement)
END_LUNA_METHODS

CLuaElement::CLuaElement(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUAELEMENT, ex)							//конструктор
{}

const char* CLuaElement::ClassName()
{
	return ( CLASS_LUAELEMENT );
}
}//namespace nrp