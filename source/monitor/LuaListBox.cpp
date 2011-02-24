#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaListBox.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"
#include "NrpText.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUALISTBOX( "CLuaListBox" );

BEGIN_LUNA_METHODS(CLuaListBox)
	LUNA_ILUALISTBOX_HEADER( CLuaListBox )
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaListBox, AddItem )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaListBox)
	LUNA_ILUALISTBOX_PROPERTIES(CLuaListBox)
	LUNA_AUTONAME_PROPERTY( CLuaListBox, "selectedObject", GetSelectedObject, PureFunction )
END_LUNA_PROPERTIES

CLuaListBox::CLuaListBox(lua_State *L, bool ex)	: ILuaListBox(L, CLASS_LUALISTBOX, ex )							//конструктор
{}

int CLuaListBox::AddItem( lua_State *L )	//добавляет текст в списко отображения
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaListBox::AddItem need 2 parameter");

	NrpText text( lua_tostring( L, 2 ) );
	void* object = _GetLuaObject< void, ILuaObject >( L, 3, true );
	
	IF_OBJECT_NOT_NULL_THEN	_object->addItem( text.ToWide(), (u32)object );			

	return 1;
}

int CLuaListBox::GetSelectedObject( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		int selected = _object->getSelected();
		if( selected >= 0 )
		{
			void* selObject = (void*)_object->getIcon( selected );
			lua_pushlightuserdata( L, selObject );
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;
}

const char* CLuaListBox::ClassName()
{
	return ( CLASS_LUALISTBOX );
}
}//namespace nrp