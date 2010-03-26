#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaElement.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{

Luna< CLuaElement >::RegType CLuaElement::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaElement ),
	/*  */
	LUNA_AUTONAME_FUNCTION( CLuaElement, RemoveChilds ),
	{0,0}
};

CLuaElement::CLuaElement(lua_State *L)	: ILuaGuiElement(L, "CLuaElement")							//конструктор
{}

int CLuaElement::RemoveChilds( lua_State* L )
{	
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaElement:RemoveChilds not need parameter" );

	gui::CNrpGUIEnvironment* guienv = (gui::CNrpGUIEnvironment*)CNrpEngine::Instance().GetGuiEnvironment(); 
	IF_OBJECT_NOT_NULL_THEN 
	{
		core::list< gui::IGUIElement* > childs = object_->getChildren();

		core::list< gui::IGUIElement* >::Iterator pIter = childs.begin();
		for( ; pIter != childs.end(); ++pIter ) 
			 guienv->addToDeletionQueue( *pIter );
	}

	return 1;			
}
}//namespace nrp