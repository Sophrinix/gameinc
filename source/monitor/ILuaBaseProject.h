#pragma once
#include "ILuaObject.h"
#include <assert.h>

namespace nrp
{

#define LUNA_ILUABASEPROJECT_HEADER(class)	LUNA_ILUAOBJECT_HEADER(class)

template< class T > class ILuaBaseProject : public ILuaObject< T >
{
public:

	ILuaBaseProject(lua_State *L, stringw luaName) : ILuaObject( L, luaName )
	{

	}

	virtual ~ILuaBaseProject(void) {};
protected:
	int SetParam_( lua_State* L, const NrpText& funcName, const NrpText& paramName )
	{
		int argc = lua_gettop( L );
		luaL_argcheck( L, argc == 2, 2, _ErrStr( NrpText(":") + funcName + "need int parameter") );

		NrpText tmpValue( lua_tostring( L, 2 ) );

		IF_OBJECT_NOT_NULL_THEN	(*object_)[ paramName ] = tmpValue;
		return 1;
	}

	template< class RTYPE, class RETTYPE > int SetParam_( lua_State* L, const NrpText&  funcName, 
		const NrpText&  paramName, 
		RETTYPE (*lua_function)(lua_State*,int) )
	{
		int argc = lua_gettop( L );
		luaL_argcheck( L, argc == 2, 2, _ErrStr( NrpText(":") + funcName + "need int parameter") );
		RTYPE lvalue = (RTYPE)lua_function( L, 2 );

		IF_OBJECT_NOT_NULL_THEN	(*object_)[ paramName ] = lvalue;
		return 1;
	}

	template< class T > T GetParam_( lua_State* L, const NrpText& funcName, const NrpText& paramName, T defValue )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, _ErrStr( NrpText(":") + funcName + " not need parameter" ) );

		IF_OBJECT_NOT_NULL_THEN	defValue = (*object_)[ paramName ].As<T>();
		return defValue;
	}

private:
	ILuaBaseProject(void);
};

}//namespace