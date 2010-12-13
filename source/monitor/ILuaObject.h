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

	static NrpText ClassName() { return L"ILuaObject"; }

protected:
	ObjectType* _object;

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
	RETCLASS* _GetLuaObject( lua_State* L, int index, bool force=false )
	{
		RETCLASS* ret = NULL;
		if( lua_isuserdata(L, index) )
		{
			ret = (RETCLASS*)lua_touserdata(L, index);
		}
		else if( lua_istable(L, index ) )
		{
			lua_pushnumber(L, 0); 	// ’отим получить значени в нулевой €чейке таблицы
			lua_gettable(L, index); // get the class table (i.e, self)

			if( force )
			{
				ILuaObject** obj = (ILuaObject**)lua_touserdata( L, -1 );
				ret = (RETCLASS*)(*obj)->GetSelf();
			}
			else
			{
				T** obj = (T**)luaL_checkudata(L, -1, T::ClassName());
				ret = (RETCLASS*)(*obj)->GetSelf();
			}

			lua_remove(L, -1);	
		}

		return ret;
	}

	int _ReadParam( lua_State* vm, int index, int mult, int start )
	{
		int ret = 0;
		if( lua_isstring( vm, index ) )
		{
			NrpText percent = lua_tostring( vm, index );

			if( percent.lastChar() == L'+' )
				percent.erase( percent.size() - 1 );
			else
				start = 0;
			
			if( percent.lastChar() == L'%' )
			{
				percent.erase( percent.size() - 1 );
				ret = start + static_cast< int >( percent.ToInt() / 100.f * mult );
			}
			else
			{
				ret = start + percent.ToInt();
			}
		}
		else if( lua_isnumber( vm, index ) )
		{
			ret = lua_tointeger( vm, index );
		}

		return ret;
	}

	irr::core::recti _ReadRect( lua_State* vm, int startParam, const gui::IGUIElement* elm )
	{
		irr::core::dimension2di sized( 0, 0 );
		if( elm )
			sized = elm->getRelativePosition().getSize();

		irr::core::recti rectangle( 0, 0, 0, 0 );

		rectangle.UpperLeftCorner.X = _ReadParam( vm, startParam, sized.Width, 0 );
		rectangle.UpperLeftCorner.Y = _ReadParam( vm, startParam + 1, sized.Height, 0 );
		rectangle.LowerRightCorner.X = _ReadParam( vm, startParam + 2, sized.Width, rectangle.UpperLeftCorner.X );
		rectangle.LowerRightCorner.Y = _ReadParam( vm, startParam + 3, sized.Height, rectangle.UpperLeftCorner.Y );

		return rectangle;
	}
	
public:

	ObjectType* GetSelf()
	{
		return _object;
	}

	ILuaObject(lua_State *L, const NrpText& className) : INrpObject( className, "" ) 
	{
		_object = _GetLuaObject<ObjectType, ILuaObject>( L, 1, true );

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