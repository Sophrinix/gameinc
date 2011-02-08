#pragma once

#include <assert.h>
#include "ILuaObject.h"
#include "NrpText.h"
#include "nrpGUIEnvironment.h"
#include "nrpEngine.h"

namespace nrp
{

#define LUNA_ILUAGUIELEMENT_HEADER(class)	LUNA_ILUAOBJECT_HEADER(class)\
											LUNA_AUTONAME_FUNCTION(class, SetVisible)\
											LUNA_AUTONAME_FUNCTION(class, GetVisible)\
											LUNA_AUTONAME_FUNCTION(class, SetName )\
											LUNA_AUTONAME_FUNCTION(class, SetAlpha )\
											LUNA_AUTONAME_FUNCTION(class, GetAlpha )\
											LUNA_AUTONAME_FUNCTION(class, Remove )\
											LUNA_AUTONAME_FUNCTION(class, SetPosition )\
											LUNA_AUTONAME_FUNCTION(class, SetRect )\
											LUNA_AUTONAME_FUNCTION(class, MoveRelative )\
											LUNA_AUTONAME_FUNCTION(class, GetSize )\
											LUNA_AUTONAME_FUNCTION(class, GetParent )\
											LUNA_AUTONAME_FUNCTION(class, SetText)\
											LUNA_AUTONAME_FUNCTION(class, GetText)\
											LUNA_AUTONAME_FUNCTION(class, GetTypeName )\
											LUNA_AUTONAME_FUNCTION(class, GetID)\
											LUNA_AUTONAME_FUNCTION(class, GetRelativePosition )\
											LUNA_AUTONAME_FUNCTION(class, GetCenter)\
											LUNA_AUTONAME_FUNCTION(class, SetEnabled )\
											LUNA_AUTONAME_FUNCTION(class, IsEnabled )\
											LUNA_AUTONAME_FUNCTION(class, GetChildCount )\
											LUNA_AUTONAME_FUNCTION(class, GetChild )\
											LUNA_AUTONAME_FUNCTION(class, RemoveChilds )\
											LUNA_AUTONAME_FUNCTION(class, SetFont )

template< class T > class ILuaGuiElement : public ILuaObject< T >
{
public:
	ILuaGuiElement(lua_State *L, NrpText luaName, bool exist) : ILuaObject( L, luaName, exist )
	{}

	int GetChildCount( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":GetChildCount not need parameter" ) ) );

		int ret = 0;
		IF_OBJECT_NOT_NULL_THEN ret = _object->getChildren().size();

		lua_pushinteger( L, ret );
		return 1;
	}

	int GetChild( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText( ":GetChild need index parameter" ) ) );

		u32 index = static_cast< u32 >( lua_tointeger( L, 2 ));

		gui::IGUIElement* ret = NULL;
		IF_OBJECT_NOT_NULL_THEN
		{
			const core::list<gui::IGUIElement*>& childs = _object->getChildren();
			bool isValid = (index >= 0 && index < childs.size());
			assert( isValid );

			ret = isValid ? *(childs.begin()+index) : NULL;
		}

		lua_pushlightuserdata( L, ret );
		return 1;
	}

	int SetFont( lua_State* L )
	{	
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText( ":SetFont need fontName parameter" ) ) );
		NrpText fontName = lua_tostring( L, 2 );

		IF_OBJECT_NOT_NULL_THEN 
		{
			gui::CNrpGUIEnvironment* guienv = dynamic_cast< gui::CNrpGUIEnvironment* >( _nrpEngine.GetGuiEnvironment() ); 
			_object->setRFont( fontName != NULL ? guienv->getFont( fontName ) : NULL );
		}

		return 1;			
	}

	int RemoveChilds( lua_State* L )
	{	
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":RemoveChilds not need parameter" ) ) );

		gui::CNrpGUIEnvironment* guienv = dynamic_cast< gui::CNrpGUIEnvironment* >( _nrpEngine.GetGuiEnvironment() ); 
		IF_OBJECT_NOT_NULL_THEN 
		{
			core::list< gui::IGUIElement* > childs = _object->getChildren();

			core::list< gui::IGUIElement* >::Iterator pIter = childs.begin();
			for( ; pIter != childs.end(); pIter++ ) 
				guienv->addToDeletionQueue( *pIter );
		}

		return 1;			
	}

	int GetParent( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":GetParent not need any parameter" ) ) );

		void* parent = NULL;

		IF_OBJECT_NOT_NULL_THEN parent = _object->getParent();
		lua_pushlightuserdata( L, parent );

		return 1;
	}

	int GetCenter( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":GetPosition not need any parameter" ) ) );

		core::position2di pos( 0, 0 );

		IF_OBJECT_NOT_NULL_THEN pos = _object->getRelativePosition().getCenter();
		lua_pushinteger( L, pos.X );
		lua_pushinteger( L, pos.Y );

		return 2;
	}

	int GetRelativePosition( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":GetPosition not need any parameter" ) ) );

		core::position2di pos( 0, 0 );

		IF_OBJECT_NOT_NULL_THEN pos = _object->getRelativePosition().UpperLeftCorner;
		lua_pushinteger( L, pos.X );
		lua_pushinteger( L, pos.Y );

		return 2;
	}

	int GetID( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":GetID not need any parameter" ) ) );

		int id = 0;

		IF_OBJECT_NOT_NULL_THEN id = _object->getID();
		lua_pushinteger( L, id );

		return 1;
	}

	int IsEnabled( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":IsEnabled not need any parameter" ) ) );

		bool enabled = false;

		IF_OBJECT_NOT_NULL_THEN enabled = _object->isEnabled();
		lua_pushboolean( L, enabled );

		return 1;
	}

	int SetText( lua_State *L )									//устанавливает новый текст
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText( ":SetText need string parameter" ) ) );

		NrpText text( lua_tostring( L, 2 ) );

		IF_OBJECT_NOT_NULL_THEN	_object->setText( text.ToWide() );

		return 1;
	}

	int GetTypeName( lua_State *L )	
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":GetTypeName not need any parameter" ) ) );

		NrpText typen("");
		IF_OBJECT_NOT_NULL_THEN 
		{
			if( _object->getType() < gui::EGUIET_COUNT )
				typen = _object->getTypeName();
			else
				typen = "";
		}

		lua_pushstring( L, typen );

		return 1;
	}

	int SetEnabled( lua_State *L )	
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText( ":SetEnabled need boolean parameter" ) ) );

		bool enabled = lua_toboolean( L, 2 ) > 0;
		IF_OBJECT_NOT_NULL_THEN _object->setEnabled( enabled );

		return 1;
	}

	int GetText( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":GetText not need any parameter" ) ) );

		NrpText text("");
		IF_OBJECT_NOT_NULL_THEN text = _object->getText();

		lua_pushstring( L, text );

		return 1;
	}

	int GetSize( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":GetSize not need any parameter" ) ) );

		core::recti rectangle; 
		IF_OBJECT_NOT_NULL_THEN	rectangle = _object->getAbsolutePosition();

		lua_pushinteger( L, rectangle.getWidth() );
		lua_pushinteger( L, rectangle.getHeight() );

		return 2;
	}

	/*
	Смещение на заданную величену относительно текущей позиции 
	*/
	int MoveRelative( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 3, 3, _ErrStr( NrpText( ":MoveRelative need 2 parameter" ) ) );

		core::position2di offset;
		offset.X = lua_tointeger( L, 2 );
		offset.Y = lua_tointeger( L, 3 );

		core::recti wndRect = _object->getAbsolutePosition();

		IF_OBJECT_NOT_NULL_THEN	_object->setRelativePosition( wndRect + offset );

		return 1;
	}

	int SetVisible(lua_State *L)							//установка видимости
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText( "::SetVisible need bool parameter" ) ) );

		assert( lua_isboolean( L, 2 ) );
		bool visible = lua_toboolean( L, 2 ) > 0;						//принимает булевое значение в качестве луа-параметра

		IF_OBJECT_NOT_NULL_THEN _object->setVisible( visible );		

		return 1;
	}

	int GetVisible(lua_State *L)							//получение видимости
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( "::self not need any parameter" ) ) );

		bool visible = false;
		IF_OBJECT_NOT_NULL_THEN visible = _object->isVisible();

		lua_pushboolean( L, visible );

		return 1;
	}

	int SetName( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText( ":SetName need string parameter " ) ) );

		NrpText name( lua_tostring( L, 2 ) );

		IF_OBJECT_NOT_NULL_THEN	_object->setName( name.ToWide() );

		return 1;
	}

	int SetAlpha( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, _ErrStr( NrpText( ":SetAlpha need int parameter " ) ) );

		int alpha = lua_tointeger( L, 2 );
		IF_OBJECT_NOT_NULL_THEN	_object->setAlphaBlend( alpha);

		return 1;
	}

	int GetAlpha( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":GetAlpha not need any parameter" ) ) );

		int alpha = 0;
		IF_OBJECT_NOT_NULL_THEN alpha = _object->getAlphaBlend();

		lua_pushinteger( L, alpha );

		return 1;
	}

	int Remove( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":Remove not need any parameter" ) ) );

		IF_OBJECT_NOT_NULL_THEN
		{
			try
			{
				_object->remove();
				_object = NULL;
			}
			catch(...)
			{
				assert( _object != NULL );
				_object  = NULL;
			}
		}

		return 1;
	}

	/*
	Изменение позиции с сохранением размеров0
	*/
	int SetPosition( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 3, 3, _ErrStr( NrpText( ":SetPosition need 2 parameter" ) ) );

		IF_OBJECT_NOT_NULL_THEN
		{
			gui::IGUIElement* elm = _object->getParent();
			core::dimension2di sides = elm ? elm->getAbsolutePosition().getSize() : core::dimension2di( 0, 0);

			core::position2di newPos;
			newPos.X = _ReadParam( L, 2, sides.Width, _object->getRelativePosition().UpperLeftCorner.X );
			newPos.Y = _ReadParam( L, 3, sides.Height, _object->getRelativePosition().UpperLeftCorner.Y );

			_object->setRelativePosition( newPos );
		}

		return 1;
	}

	/*
	Изменение геoметрии окна
	*/
	int SetRect( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 5, 5, _ErrStr( NrpText(  ":SetRect need 2 parameter" ) ) );

		core::recti newRect;
		newRect.UpperLeftCorner.X = lua_tointeger( L, 2 );
		newRect.UpperLeftCorner.Y = lua_tointeger( L, 3 );
		newRect.LowerRightCorner.X = lua_tointeger( L, 4 );
		newRect.LowerRightCorner.Y = lua_tointeger( L, 5 );

		IF_OBJECT_NOT_NULL_THEN	_object->setRelativePosition( newRect );

		return 1;
	}
};

}//namespace nrp