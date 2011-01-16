#pragma once
#include <lua.hpp>
#include <luna.h>
#include "ILuaObject.h"

namespace nrp
{

class CLuaAutoScript
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaAutoScript)

	CLuaAutoScript( lua_State *L, bool );
	int AddUserToInvention( lua_State* L );
	int AddGameEngineToCompany( lua_State* L );
	int AddUserToGameProject( lua_State* L );
	//int __index( lua_State* L ) { return 1; };

	static const char* ClassName();

	bool IsExist() { return _isExisting; }
	bool IsPrecious() { return _isPrecious; }

private:

	bool _isExisting; // This is used by Luna to see whether it's been created by createFromExisting.  Don't set it.
	bool _isPrecious; // This is used to tell Luna not to garbage collect the object, in case other objects might reference it.  Set it in your classes constructor.

};

}//namespace nrp