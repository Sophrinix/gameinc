#pragma once
#include "ILuaObject.h"
#include <assert.h>

namespace nrp
{

#define LUNA_ILUABASEPROJECT_HEADER(class)	LUNA_ILUAOBJECT_HEADER(class)

template< class T > class ILuaBaseProject : public ILuaObject< T >
{
public:

	ILuaBaseProject(lua_State *L, std::string luaName) : ILuaObject( L, luaName )
	{

	}

	virtual ~ILuaBaseProject(void) {};
protected:
	int SetParam_( lua_State* L, std::string funcName, std::string paramName )
	{
		int argc = lua_gettop( L );
		luaL_argcheck( L, argc == 2, 2, ("Function " + ObjectTypeName() + ":" + funcName + "need int parameter").c_str() );

		const char* tmpValue = lua_tostring( L, 2 );
		assert( tmpValue != NULL );
		IF_OBJECT_NOT_NULL_THEN	object_->SetValue<std::string>( paramName, tmpValue );
		return 1;
	}

	template< class RTYPE, class RETTYPE > int SetParam_( lua_State* L, std::string funcName, 
		std::string paramName, 
		RETTYPE (*lua_function)(lua_State*,int) )
	{
		int argc = lua_gettop( L );
		luaL_argcheck( L, argc == 2, 2, ("Function " + ObjectTypeName() + ":" + funcName + "need int parameter").c_str() );
		RTYPE lvalue = (RTYPE)lua_function( L, 2 );

		IF_OBJECT_NOT_NULL_THEN	object_->SetValue<RTYPE>( paramName, lvalue );
		return 1;
	}

	template< class T > T GetParam_( lua_State* L, std::string funcName, std::string paramName, T defValue )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ("Function " + ObjectTypeName() + ":" + funcName + " not need parameter").c_str() );

		IF_OBJECT_NOT_NULL_THEN	defValue = object_->GetValue<T>( paramName );
		return defValue;
	}

private:
	ILuaBaseProject(void);
};

}//namespace