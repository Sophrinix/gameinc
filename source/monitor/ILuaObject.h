#pragma once

#include <lua.hpp>
#include <luna.h>
#include <rect.h>
#include <assert.h>

#include "INrpObject.h"
#include "nrptext.h"

#define IF_OBJECT_NOT_NULL_THEN if( _object == NULL ) DebugReport( __FILEW__, __LINE__, "Access null object" ); else

#define LUNA_ILUAOBJECT_HEADER(class) LUNA_AUTONAME_FUNCTION(class,	SetObject),\
									  LUNA_AUTONAME_FUNCTION(class, Self),\
									  LUNA_AUTONAME_FUNCTION(class, Empty)

namespace nrp
{

template < class ObjectType > class ILuaObject : public INrpObject
{
private:
	ILuaObject() {};

protected:
	ObjectType* _object;

	irr::core::recti _ReadRect( lua_State* vm, size_t startParam )
	{
		irr::core::recti rectangle( 0, 0, 0, 0 );

		rectangle.UpperLeftCorner.X = lua_tointeger( vm, startParam );
		rectangle.UpperLeftCorner.Y = lua_tointeger( vm, startParam + 1 );
		rectangle.LowerRightCorner.X = lua_tointeger( vm, startParam + 2 );
		rectangle.LowerRightCorner.Y = lua_tointeger( vm, startParam + 3 );

		return rectangle;
	}

	NrpText _ErrStr( const char* str )
	{
		return ObjectTypeName() + NrpText( str );
	}

	NrpText _ErrEmptyObject()
	{
		return NrpText( "Assign empty object in " ) + ObjectTypeName();
	}

	NrpText _ErrNotNeedParam() 
	{ 
		return ObjectTypeName() + NrpText( ":Self not need any parameter" );
	}

	template< class RETCLASS, class T>
	RETCLASS* _GetObjectFromTable( lua_State* L, int index, int retIndex )
	{
		assert( lua_istable(L, index) );
		if (!lua_istable(L, index))
			return 0;

		lua_pushnumber(L, 0); 	// ’отим получить значени в нулевой €чейке таблицы
		lua_gettable(L, index); // get the class table (i.e, self)

		T** obj = (T**)(luaL_checkudata(L, retIndex, T::ClassName()));
		lua_remove(L, -1);

		return (RETCLASS*)(*obj)->GetSelf();
	}

	ILuaObject* _GetLuaObject( lua_State* L, int index, int retIndex )
	{
		assert( lua_istable(L, index) );
		if (!lua_istable(L, index))
			return 0;

		lua_pushnumber( L, 0 );
		lua_gettable( L, index );

		//lua_Debug ar;
		//lua_getinfo( L, ">S", &ar );
		ILuaObject** obj = (ILuaObject**)lua_touserdata(L, retIndex);
		lua_remove(L, -1);

		return *obj;
	}
	
public:

	ObjectType* GetSelf()
	{
		return _object;
	}

	ILuaObject(lua_State *L, const NrpText& className) : INrpObject( className, "" ) 
	{
		if( lua_islightuserdata( L, 1 ) )
		{
			_object = (ObjectType*)lua_touserdata(L, 1);
		}
		else if( lua_istable( L, 1 ) )
		{
			_object = _GetLuaObject( L, 1, -1 )->_object;
		}

		if( _object == NULL )
			DebugReport( __FILEW__, __LINE__, _ErrEmptyObject() );

	}

	virtual int SetObject(lua_State *L)
	{
		_object = (ObjectType*)lua_touserdata(L, 2);

		if( _object == NULL )
			DebugReport( __FILEW__, __LINE__, _ErrEmptyObject() );

		return 1;
	}

	virtual int Self( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrNotNeedParam() );

		lua_pushlightuserdata( L, (void*)_object );

		return 1;
	}

	virtual int Empty( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrNotNeedParam() );

		lua_pushinteger( L, _object == NULL ? 1 : 0 );

		return 1;
	}

	virtual int DebugReport( const NrpText& fileName, int lineNumber, const NrpText& text )
	{
#ifdef _DEBUG
		Log(HW) << fileName << ":" << lineNumber << " Error" << text;
#endif
		return 1;
	}
};

}//namespace nrp