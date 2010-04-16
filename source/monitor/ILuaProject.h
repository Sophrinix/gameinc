#pragma once
#include "ILuaObject.h"

namespace nrp
{

#define LUNA_ILUAPROJECT_HEADER(class)	LUNA_ILUAOBJECT_HEADER(class)

template< class T > class ILuaProject : public ILuaObject< T >
{
public:

	ILuaProject(lua_State *L, std::string luaName) : ILuaObject( L, luaName )
	{

	}

	virtual ~ILuaProject(void) {};
protected:

	template< class T > T GetParam_( lua_State* L, std::string funcName, std::string paramName, T defValue )
	{
		int argc = lua_gettop(L);
		luaL_argcheck(L, argc == 1, 1, ("Function " + ClassName() + ":" + funcName + " not need parameter").c_str() );

		IF_OBJECT_NOT_NULL_THEN	defValue = object_->GetValue<T>( paramName );
		return defValue;
	}

private:
	ILuaProject(void);
};

}//namespace