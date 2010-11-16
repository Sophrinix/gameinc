#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaElement.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAELEMENT( "CLuaElement" );

Luna< CLuaElement >::RegType CLuaElement::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaElement ),
	/*  */
	//LUNA_AUTONAME_FUNCTION( CLuaElement, RemoveChilds ),
	{0,0}
};

CLuaElement::CLuaElement(lua_State *L)	: ILuaGuiElement(L, "CLuaElement")							//конструктор
{}

const char* CLuaElement::ClassName()
{
	return ( CLASS_LUAELEMENT );
}
}//namespace nrp