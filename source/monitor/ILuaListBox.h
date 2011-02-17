#pragma once

#include <assert.h>
#include "ILuaGuiElement.h"

namespace nrp
{

#define LUNA_ILUALISTBOX_HEADER(class)	LUNA_ILUAGUIELEMENT_HEADER(class)\
										LUNA_AUTONAME_FUNCTION(class, SetItemBgColor )\
										LUNA_AUTONAME_FUNCTION(class, SetFontFromSize )\
										LUNA_AUTONAME_FUNCTION(class, SetItemTextColor )\
										LUNA_AUTONAME_FUNCTION(class, GetSelected )\
										LUNA_AUTONAME_FUNCTION(class, SetSelected )\
										LUNA_AUTONAME_FUNCTION(class, Clear )\
										LUNA_AUTONAME_FUNCTION(class, SetItemHeigth )

#define LUNA_ILUALISTBOX_PROPERTIES(class) LUNA_ILUAGUIELEMENT_PROPERTIES(class)

template< class T >
class ILuaListBox : public ILuaGuiElement<T>
{
public:
	ILuaListBox(lua_State *L, const NrpText& luaName, bool ex) : ILuaGuiElement( L, luaName, ex )
	{

	}

	int SetItemBgColor( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 6, 6, _ErrStr( NrpText(":GetSelectedObject need index, color(a,r,g,b) parameter") ) );

		int index = lua_tointeger( L, 2 );
		video::SColor color( lua_tointeger( L, 3 ), lua_tointeger( L, 4 ), 
			lua_tointeger( L, 5 ), lua_tointeger( L, 6 ) );

		IF_OBJECT_NOT_NULL_THEN _object->setItemOverrideColor( index, gui::EGUI_LBC_BACKGROUND, color );

		return 1;
	}	

	int SetItemHeigth( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText(":SetSelected need int parameter") ) );

		int height = lua_tointeger( L, 2 );
		assert( height > 0 && height < 200 );

		IF_OBJECT_NOT_NULL_THEN	_object->setItemHeight( height );			

		return 1;
	}


	int Clear( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText(":Clear not need any parameter" ) ) );

		IF_OBJECT_NOT_NULL_THEN	_object->clear();			

		return 1;
	}

	int SetSelected( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText( ":SetSelected need int parameter" ) ) );

		int selected = lua_tointeger( L, 2 );

		IF_OBJECT_NOT_NULL_THEN	_object->setSelected( selected );			

		return 1;
	}

	int GetSelected( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":GetSelected not need any parameter" ) ) );

		int selected = -1;

		IF_OBJECT_NOT_NULL_THEN selected = _object->getSelected();
		lua_pushinteger( L, selected );

		return 1;
	}

	int SetFontFromSize( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText( ":SetFontFromSize need size parameter" ) ) );

		int size = lua_tointeger( L, 2 );

		IF_OBJECT_NOT_NULL_THEN 
		{
			irr::gui::IGUIFont* font = _nrpEngine.GetGuiEnvironment()->getFont( NrpText("font_") + NrpText( size ) );
			_object->setRFont( font );
		}

		return 1;
	}

	int SetItemTextColor( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 6, 6, _ErrStr( NrpText( ":SetTextColor need size parameter") ) );

		int index = lua_tointeger( L, 2 );
		irr::video::SColor color( lua_tointeger( L, 3 ), 
			lua_tointeger( L, 4 ),
			lua_tointeger( L, 5 ),
			lua_tointeger( L, 6 ) );

		IF_OBJECT_NOT_NULL_THEN _object->setItemOverrideColor( index, gui::EGUI_LBC_TEXT, color );

		return 1;
	}
};

}//end namespace nrp