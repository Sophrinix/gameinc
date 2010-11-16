#pragma once

#include <lua.hpp>
#include <luna.h>
#include <rect.h>
#include <assert.h>

#include "INrpObject.h"
#include "nrptext.h"

#define IF_OBJECT_NOT_NULL_THEN if( object_ == NULL ) DebugReport( __FILE__, __LINE__, "Access null object" ); else

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
	ObjectType* object_;

	irr::core::recti _ReadRect( lua_State* vm, size_t startParam )
	{
		irr::core::recti rectangle( 0, 0, 0, 0 );

		rectangle.UpperLeftCorner.X = lua_tointeger( vm, startParam );
		rectangle.UpperLeftCorner.Y = lua_tointeger( vm, startParam + 1 );
		rectangle.LowerRightCorner.X = lua_tointeger( vm, startParam + 2 );
		rectangle.LowerRightCorner.Y = lua_tointeger( vm, startParam + 3 );

		return rectangle;
	}

	const char* _ErrStr( const char* str )
	{
		return NrpText( ObjectTypeName() + NrpText( str ) ).ToStr();
	}

	const char* _ErrEmptyObject()
	{
		return NrpText( NrpText( "Assign empty object in " ) + ObjectTypeName() ).ToStr();
	}

	const char* _ErrNotNeedParam() 
	{ 
		return NrpText( ObjectTypeName() + NrpText( ":Self not need any parameter" ) ).ToStr();
	}
	
public:
	
	ILuaObject(lua_State *L, stringw ClassName) : INrpObject( ClassName, "" ) 
	{
		object_ = (ObjectType*)lua_touserdata(L, 1);

		if( object_ == NULL )
			DebugReport( __FILE__, __LINE__, _ErrEmptyObject() );
	}

	virtual int SetObject(lua_State *L)
	{
		object_ = (ObjectType*)lua_touserdata(L, 2);

		if( object_ == NULL )
			DebugReport( __FILE__, __LINE__, _ErrEmptyObject() );

		return 1;
	}

	virtual int Self( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrNotNeedParam() );

		lua_pushlightuserdata( L, (void*)object_ );

		return 1;
	}

	virtual int Empty( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrNotNeedParam() );

		lua_pushinteger( L, object_ == NULL ? 1 : 0 );

		return 1;
	}

	virtual int DebugReport( const char* fileName, const int lineNumber, const char* text )
	{
#ifdef _DEBUG
		Log(HW) << fileName << ":" << lineNumber << " Error" << text;
#endif
		return 1;
	}
};

}//namespace nrp