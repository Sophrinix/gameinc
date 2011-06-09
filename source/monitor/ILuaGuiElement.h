#pragma once

#include <assert.h>
#include "ILuaObject.h"
#include "NrpText.h"
#include "nrpGUIEnvironment.h"
#include "nrpEngine.h"

namespace nrp
{

#define LUNA_ILUAGUIELEMENT_HEADER(class)	LUNA_AUTONAME_FUNCTION(class, Remove )\
											LUNA_AUTONAME_FUNCTION(class, SetPosition )\
											LUNA_AUTONAME_FUNCTION(class, SetRect )\
											LUNA_AUTONAME_FUNCTION(class, MoveRelative )\
											LUNA_AUTONAME_FUNCTION(class, GetRelativePosition )\
											LUNA_AUTONAME_FUNCTION(class, GetChild )\
											LUNA_AUTONAME_FUNCTION(class, GetCenter )\
											LUNA_AUTONAME_FUNCTION(class, GetSize)\
											LUNA_AUTONAME_FUNCTION(class, RemoveChilds )\
											LUNA_AUTONAME_FUNCTION(class, SetRectAlignment )\
											LUNA_AUTONAME_FUNCTION(class, SetMaxSize)

#define LUNA_ILUAGUIELEMENT_PROPERTIES(class) LUNA_ILUAOBJECT_PROPERTIES(class)\
											  LUNA_AUTONAME_PROPERTY(class, "parent", GetParent, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "text", GetText, SetText )\
											  LUNA_AUTONAME_PROPERTY(class, "visible", GetVisible, SetVisible)\
											  LUNA_AUTONAME_PROPERTY(class, "enabled", IsEnabled, SetEnabled )\
											  LUNA_AUTONAME_PROPERTY(class, "alpha", GetAlpha, SetAlpha )\
											  LUNA_AUTONAME_PROPERTY(class, "size", GetSize, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "typeName", GetTypeName, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "id", GetID, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "font", PureFunction, SetFont )\
											  LUNA_AUTONAME_PROPERTY(class, "childCount", GetChildCount, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "name", GetName, SetName )\
											  LUNA_AUTONAME_PROPERTY(class, "width", GetWidth, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "height", GetHeight, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "left", GetLeft, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "screenLeft", GetScreenLeft, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "right", GetRight, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "screenRight", GetScreenRight, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "top", GetTop, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "screenTop", GetScreenTop, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "bottom", GetBottom, PureFunction )\
											  LUNA_AUTONAME_PROPERTY(class, "screenBottom", GetScreenBottom, PureFunction )
														

template< class T > class ILuaGuiElement : public ILuaObject< T >
{
public:
	ILuaGuiElement(lua_State *L, NrpText luaName, bool exist) : ILuaObject( L, luaName, exist )
	{}

	int GetScreenRight( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushinteger( L, _object->getAbsolutePosition().LowerRightCorner.X );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetRight( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushinteger( L, _object->getRelativePosition().LowerRightCorner.X );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetTop( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushinteger( L, _object->getRelativePosition().UpperLeftCorner.Y );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetScreenTop( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushinteger( L, _object->getAbsolutePosition().UpperLeftCorner.Y );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetHeight( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushinteger( L, _object->getAbsolutePosition().getHeight() );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetName( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			NrpText name = _object->getName();
			lua_pushstring( L, name );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}
	
	int GetWidth( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushinteger( L, _object->getAbsolutePosition().getWidth() );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetLeft( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushinteger( L, _object->getRelativePosition().UpperLeftCorner.X );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetScreenLeft( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushinteger( L, _object->getAbsolutePosition().UpperLeftCorner.X );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetBottom( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushinteger( L, _object->getRelativePosition().LowerRightCorner.Y );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetScreenBottom( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushinteger( L, _object->getAbsolutePosition().LowerRightCorner.Y );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetChildCount( lua_State* L )
	{
		
		IF_OBJECT_NOT_NULL_THEN 
		{
			int ret = _object->getChildren().size();
			lua_pushinteger( L, ret );
			return 1;
		}

		lua_pushnil( L );
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

	int SetRectAlignment( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 5, 5, "Function CLuaLabel::SetTextAlignment need bool, bool parameter");

		int left = lua_tointeger( L, 2 );
		int right = lua_tointeger( L, 3 );
		int top = lua_tointeger( L, 4 );
		int bottom = lua_tointeger( L, 5 );

		IF_OBJECT_NOT_NULL_THEN	_object->setAlignment( gui::EGUI_ALIGNMENT(left), gui::EGUI_ALIGNMENT(right),
													   gui::EGUI_ALIGNMENT(top), gui::EGUI_ALIGNMENT(bottom) );

		return 1;
	}

	int SetFont( lua_State* L )
	{	
		assert( lua_isstring( L, -1 ) );
		IF_OBJECT_NOT_NULL_THEN 
		{
			NrpText fontName = lua_tostring( L, -1 );
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
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushlightuserdata( L, _object->getParent() );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetCenter( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText( ":GetPosition not need any parameter" ) ) );

		IF_OBJECT_NOT_NULL_THEN
		{
			core::position2di pos = _object->getRelativePosition().getCenter();
			lua_pushinteger( L, pos.X ); lua_pushinteger( L, pos.Y );
			return 2;
		}

		lua_pushnil( L ); lua_pushnil( L );
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
		IF_OBJECT_NOT_NULL_THEN
		{
			int id = _object->getID();
			lua_pushinteger( L, id );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int IsEnabled( lua_State *L )
	{
		IF_OBJECT_NOT_NULL_THEN
		{
			bool enabled = _object->isEnabled();
			lua_pushboolean( L, enabled );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int SetText( lua_State *L )									//устанавливает новый текст
	{
		assert( lua_isstring( L, -1 ) );
		NrpText text( lua_tostring( L, -1 ) );
		IF_OBJECT_NOT_NULL_THEN	_object->setText( text.ToWide() );
		return 1;
	}

	int GetTypeName( lua_State *L )	
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushstring( L, _object->getTypeName() );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int SetEnabled( lua_State *L )	
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			bool enabled = lua_toboolean( L, -1 ) > 0;
			_object->setEnabled( enabled );
		}

		return 0;
	}

	int GetText( lua_State *L )
	{
		IF_OBJECT_NOT_NULL_THEN
		{
			NrpText text = _object->getText();
			lua_pushstring( L, text );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int GetSize( lua_State *L )
	{
		IF_OBJECT_NOT_NULL_THEN
		{
			core::recti rectangle = _object->getAbsolutePosition();
			lua_pushinteger( L, rectangle.getWidth() );
			lua_pushinteger( L, rectangle.getHeight() );
			return 2;
		}

		lua_pushnil( L ); lua_pushnil( L );
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

	int SetMaxSize( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 3, 3, _ErrStr( NrpText( ":SetMaxSize need 2 parameter" ) ) );

		IF_OBJECT_NOT_NULL_THEN
		{
			core::dimension2du offset;
			offset.Width = _ReadParam( L, 2, _object->getParent()->getAbsolutePosition().getWidth(), 0 );
			offset.Height = _ReadParam( L, 3, _object->getParent()->getAbsolutePosition().getHeight(), 0 );

			_object->setMaxSize( offset );
		}

		return 1;
	}

	int SetVisible(lua_State *L)							//установка видимости
	{
		assert( lua_isboolean( L, -1 ) );
		IF_OBJECT_NOT_NULL_THEN _object->setVisible( lua_toboolean( L, -1 ) > 0 );		
		return 1;
	}

	int GetVisible(lua_State *L)							//получение видимости
	{
		IF_OBJECT_NOT_NULL_THEN 
		{
			lua_pushboolean( L, _object->isVisible() );
			return 1;
		}

		lua_pushnil( L );
		return 1;
	}

	int SetName( lua_State* L )
	{
		assert( lua_isstring( L, -1 ));
		IF_OBJECT_NOT_NULL_THEN
		{
			NrpText name( lua_tostring( L, -1 ) );
			_object->setName( name.ToWide() );
		}

		return 0;
	}

	int SetAlpha( lua_State* L )
	{
		assert( lua_isnumber( L, -1 ) );
		IF_OBJECT_NOT_NULL_THEN
		{
			int alpha = lua_tointeger( L, -1 );
			_object->setAlphaBlend( alpha );
		}

		return 1;
	}

	int GetAlpha( lua_State* L )
	{
		IF_OBJECT_NOT_NULL_THEN
		{
			int alpha = _object->getAlphaBlend();
			lua_pushinteger( L, alpha );
			return 1;
		}

		lua_pushnil( L );
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
				//безопасное удаление
				if( gui::CNrpGUIEnvironment* env = dynamic_cast< gui::CNrpGUIEnvironment* >( _nrpEngine.Instance().GetGuiEnvironment() ) )
				{
					env->addToDeletionQueue( _object );
				}
				else
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