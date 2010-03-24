#pragma once

#include <lua.hpp>
#include <luna.h>
#include <string>
#include "INrpObject.h"
#include <rect.h>

#define FUNC_NOT_NEED_PARAM (std::string( "Function " ) + ClassName() + std::string( "::self not need any parameter" )).c_str()
#define ASSIGN_EMPTY_OBJECT (std::string( "Assign empty object in " ) + ClassName() ).c_str()
#define IF_OBJECT_NOT_NULL_THEN if( object_ == NULL ) DebugReport( __FILE__, __LINE__, "Access null object" ); else

#define LUNA_ILUAOBJECT_HEADER(class) LUNA_AUTONAME_FUNCTION(class,	  SetObject),\
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

	int GetInteger( lua_State* vm, size_t paramNumber )
	{
		if( lua_typename( vm, paramNumber ) == "number" )
		{

		}
		else
		{
			char errStr[ MAX_PATH ];
			sprintf_s( errStr, MAX_PATH, "%s:%d Error: %s\n", fileName, lineNumber, text );
			OutputDebugString( errStr );
			return 0;
		}
	}

	irr::core::recti ReadRect_( lua_State* vm, size_t startParam )
	{
		irr::core::recti rectangle( 0, 0, 0, 0 );

		rectangle.UpperLeftCorner.X = lua_tointeger( vm, startParam );
		rectangle.UpperLeftCorner.Y = lua_tointeger( vm, startParam + 1 );
		rectangle.LowerRightCorner.X = lua_tointeger( vm, startParam + 2 );
		rectangle.LowerRightCorner.Y = lua_tointeger( vm, startParam + 3 );

		return rectangle;
	}

	
public:
	
	ILuaObject(lua_State *L, std::string className) : INrpObject( className, "" ) 
	{
		object_ = (ObjectType*)lua_touserdata(L, 1);

		if( object_ == NULL )
			DebugReport( __FILE__, __LINE__, ASSIGN_EMPTY_OBJECT );
	}

	virtual int SetObject(lua_State *L)
	{
		object_ = (ObjectType*)lua_touserdata(L, 2);

		if( object_ == NULL )
			DebugReport( __FILE__, __LINE__, ASSIGN_EMPTY_OBJECT );

		return 1;
	}

	virtual int Self( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, FUNC_NOT_NEED_PARAM );

		lua_pushlightuserdata( L, (void*)object_ );

		return 1;
	}

	virtual int Empty( lua_State *L )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, FUNC_NOT_NEED_PARAM );

		lua_pushinteger( L, object_ == NULL ? 1 : 0 );

		return 1;
	}

	virtual int DebugReport( const char* fileName, const int lineNumber, const char* text )
	{
#ifdef _DEBUG
		char errStr[ MAX_PATH ];
		sprintf_s( errStr, MAX_PATH, "%s:%d Error: %s\n", fileName, lineNumber, text );
		OutputDebugString( errStr );
#endif
		return 1;
	}
};

}//namespace nrp