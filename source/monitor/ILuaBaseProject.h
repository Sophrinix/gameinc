#pragma once
#include "ILuaObject.h"
#include <assert.h>

namespace nrp
{

const NrpText PROP("property");

#define LUNA_ILUABASEPROJECT_PROPERTIES(class)	LUNA_ILUAOBJECT_PROPERTIES(class)

template< class T > class ILuaBaseProject : public ILuaObject< T >
{
public:

	ILuaBaseProject(lua_State *L, const NrpText& luaName, bool ex) : ILuaObject( L, luaName, ex )
	{

	}

	virtual ~ILuaBaseProject(void) {};
protected:
	int SetParam_( lua_State* L, const NrpText& funcName, const NrpText& paramName )
	{
		int index = -1;
		if( funcName != PROP )
		{
			int argc = lua_gettop( L );
			luaL_argcheck( L, argc == 2, 2, _ErrStr( NrpText(":") + funcName + "need string parameter") );
			index = 2;
		}

		NrpText tmpValue( lua_tostring( L, index ) );

		IF_OBJECT_NOT_NULL_THEN	(*_object)[ paramName ] = tmpValue;
		return 0;
	}

	template< class RTYPE, class RETTYPE > int SetParam_( lua_State* L, const NrpText&  funcName, 
		const NrpText&  paramName, 
		RETTYPE (*lua_function)(lua_State*,int) )
	{
		int index = -1;
		if( funcName != PROP )
		{
			int argc = lua_gettop( L );
			luaL_argcheck( L, argc == 2, 2, _ErrStr( NrpText(":") + funcName + "need one parameter") );
			index = 2;
		}
		RTYPE lvalue = (RTYPE)lua_function( L, index );

		IF_OBJECT_NOT_NULL_THEN	(*_object)[ paramName ] = lvalue;
		return 0;
	}

	template< class T > T GetParam_( lua_State* L, const NrpText& funcName, const NrpText& paramName, T defValue )
	{
		if( funcName != PROP )
		{
			int argc = lua_gettop(L);
			luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText(":") + funcName + " not need parameter" ) );
		}

		IF_OBJECT_NOT_NULL_THEN	defValue = (*_object)[ paramName ].As<T>();
		return defValue;
	}

private:
	ILuaBaseProject(void);
};

}//namespace