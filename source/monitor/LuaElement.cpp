#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaElement.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{

Luna< CLuaElement >::RegType CLuaElement::methods[] =			//���������� ������
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaElement ),
	/*  */
	//LUNA_AUTONAME_FUNCTION( CLuaElement, RemoveChilds ),
	{0,0}
};

CLuaElement::CLuaElement(lua_State *L)	: ILuaGuiElement(L, "CLuaElement")							//�����������
{}

}//namespace nrp