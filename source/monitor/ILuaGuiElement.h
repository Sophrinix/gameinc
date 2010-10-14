#pragma once

#include <assert.h>
#include "ILuaObject.h"
#include "StrConversation.h"

namespace nrp
{

#define LUNA_ILUAGUIELEMENT_HEADER(class)	LUNA_ILUAOBJECT_HEADER(class),\
											LUNA_AUTONAME_FUNCTION(class, SetVisible),\
											LUNA_AUTONAME_FUNCTION(class, GetVisible),\
											LUNA_AUTONAME_FUNCTION(class, SetName ),\
											LUNA_AUTONAME_FUNCTION(class, SetAlpha ),\
											LUNA_AUTONAME_FUNCTION(class, GetAlpha ),\
											LUNA_AUTONAME_FUNCTION(class, Remove ),\
											LUNA_AUTONAME_FUNCTION(class, SetPosition ),\
											LUNA_AUTONAME_FUNCTION(class, SetRect ),\
											LUNA_AUTONAME_FUNCTION(class, MoveRelative ),\
											LUNA_AUTONAME_FUNCTION(class, GetSize ),\
											LUNA_AUTONAME_FUNCTION(class, GetParent ),\
											LUNA_AUTONAME_FUNCTION(class, SetText),\
											LUNA_AUTONAME_FUNCTION(class, GetText),\
											LUNA_AUTONAME_FUNCTION(class, GetTypeName ),\
											LUNA_AUTONAME_FUNCTION(class, GetID),\
											LUNA_AUTONAME_FUNCTION(class, GetRelativePosition ),\
											LUNA_AUTONAME_FUNCTION(class, SetEnabled ),\
											LUNA_AUTONAME_FUNCTION(class, IsEnabled )

template< class T > class ILuaGuiElement : public ILuaObject< T >
{
public:
	ILuaGuiElement(lua_State *L, std::string luaName) : ILuaObject( L, luaName )
	{

	}

	int GetParent( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ClassName() + ":GetParent not need any parameter" ).c_str() );

		void* parent = NULL;

		IF_OBJECT_NOT_NULL_THEN parent = object_->getParent();
		lua_pushlightuserdata( L, parent );

		return 1;
	}

	int GetRelativePosition( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ClassName() + ":GetPosition not need any parameter" ).c_str() );

		core::position2di pos( 0, 0 );

		IF_OBJECT_NOT_NULL_THEN pos = object_->getRelativePosition().UpperLeftCorner;
		lua_pushinteger( L, pos.X );
		lua_pushinteger( L, pos.Y );

		return 2;
	}

	int GetID( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ClassName() + ":GetID not need any parameter" ).c_str() );

		int id = 0;

		IF_OBJECT_NOT_NULL_THEN id = object_->getID();
		lua_pushinteger( L, id );

		return 1;
	}

	int IsEnabled( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ClassName() + ":IsEnabled not need any parameter" ).c_str() );

		bool enabled = false;

		IF_OBJECT_NOT_NULL_THEN enabled = object_->isEnabled();
		lua_pushboolean( L, enabled );

		return 1;
	}

	int SetText( lua_State *L )									//устанавливает новый текст
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, ("Function " + ClassName() + ":SetText need string parameter").c_str());

		const char* text = lua_tostring( L, 2 );
		assert( text != NULL );

		IF_OBJECT_NOT_NULL_THEN	
		{
			if( text )
				object_->setText( StrToWide( text ).c_str() );
		}

		return 1;
	}

	int GetTypeName( lua_State *L )	
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ("Function " + ClassName() + ":GetTypeName not need any parameter").c_str() );

		std::string typen("");
		IF_OBJECT_NOT_NULL_THEN typen = object_->getTypeName();

		lua_pushstring( L, typen.c_str() );

		return 1;
	}

	int SetEnabled( lua_State *L )	
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, ("Function " + ClassName() + ":SetEnabled need boolean parameter").c_str() );

		bool enabled = lua_toboolean( L, 2 ) > 0;
		IF_OBJECT_NOT_NULL_THEN object_->setEnabled( enabled );

		return 1;
	}

	int GetText( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ("Function " + ClassName() + ":GetText not need any parameter").c_str() );

		std::string text("");
		IF_OBJECT_NOT_NULL_THEN text = WideToStr( object_->getText() );

		lua_pushstring( L, text.c_str() );

		return 1;
	}

	int GetSize( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ClassName() + ":GetSize not need any parameter" ).c_str() );

		core::recti rectangle; 
		IF_OBJECT_NOT_NULL_THEN	rectangle = object_->getAbsolutePosition();

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
		luaL_argcheck(L, argc == 3, 3, ( "Function " + ClassName() + ":MoveRelative need 2 parameter" ).c_str() );

		core::position2di offset;
		offset.X = lua_tointeger( L, 2 );
		offset.Y = lua_tointeger( L, 3 );

		core::recti wndRect = object_->getAbsolutePosition();

		IF_OBJECT_NOT_NULL_THEN	object_->setRelativePosition( wndRect + offset );

		return 1;
	}

	int SetVisible(lua_State *L)							//установка видимости
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, ( "Function " + ClassName() + "::SetVisible need bool parameter" ).c_str() );

		bool visible = lua_toboolean( L, 2 ) > 0;						//принимает булевое значение в качестве луа-параметра

		IF_OBJECT_NOT_NULL_THEN object_->setVisible( visible );		

		return 1;
	}

	int GetVisible(lua_State *L)							//получение видимости
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ClassName() + "::self not need any parameter" ).c_str() );

		bool visible = false;
		IF_OBJECT_NOT_NULL_THEN visible = object_->isVisible();

		lua_pushboolean( L, visible );

		return 1;
	}

	int SetName( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, ( "Function " + ClassName() + ":SetName need string parameter " ).c_str() );

		const char* name = lua_tostring( L, 2 );
		assert( name != NULL );

		IF_OBJECT_NOT_NULL_THEN	object_->setName( StrToWide( name ).c_str() );

		return 1;
	}

	int SetAlpha( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 2, 2, ( "Function " + ClassName() + "SetAlpha need int parameter " ).c_str() );

		int alpha = lua_tointeger( L, 2 );
		IF_OBJECT_NOT_NULL_THEN	object_->setAlphaBlend( alpha);

		return 1;
	}

	int GetAlpha( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ClassName() + ":GetAlpha not need any parameter" ).c_str() );

		int alpha = 0;
		IF_OBJECT_NOT_NULL_THEN alpha = object_->getAlphaBlend();

		lua_pushinteger( L, alpha );

		return 1;
	}

	int Remove( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ( "Function " + ClassName() + ":Remove not need any parameter" ).c_str() );

		IF_OBJECT_NOT_NULL_THEN
		{
			try
			{
				object_->remove();
				object_ = NULL;
			}
			catch(...)
			{
				assert( object_ != NULL );
				object_  = NULL;
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
		luaL_argcheck(L, argc == 3, 3, ( "Function " + ClassName() + ":SetPosition need 2 parameter" ).c_str() );

		core::position2di newPos;
		newPos.X = lua_tointeger( L, 2 );
		newPos.Y = lua_tointeger( L, 3 );

		core::recti wndRect = object_->getAbsolutePosition();
		core::recti newRect( newPos.X, newPos.Y, newPos.X + wndRect.getWidth(), newPos.Y + wndRect.getHeight() );

		IF_OBJECT_NOT_NULL_THEN object_->setRelativePosition( newRect );

		return 1;
	}

	/*
	Изменение геoметрии окна
	*/
	int SetRect( lua_State* L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 5, 5, ( "Function " + ClassName() + ":SetRect need 2 parameter" ).c_str() );

		core::recti newRect;
		newRect.UpperLeftCorner.X = lua_tointeger( L, 2 );
		newRect.UpperLeftCorner.Y = lua_tointeger( L, 3 );
		newRect.LowerRightCorner.X = lua_tointeger( L, 4 );
		newRect.LowerRightCorner.Y = lua_tointeger( L, 5 );

		IF_OBJECT_NOT_NULL_THEN	object_->setRelativePosition( newRect );

		return 1;
	}

};

}//namespace nrp