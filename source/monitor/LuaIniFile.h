#pragma once

#include "ILuaObject.h"

namespace nrp
{
	class IniFile;
}

namespace nrp
{
CLASS_NAME CLASS_LUAINIFILE( "CLuaIniFile" );

class CLuaIniFile : public ILuaObject<nrp::IniFile>
{
	std::string fileName_;
public:
	static Luna<CLuaIniFile>::RegType methods[];				//методы обертки

	CLuaIniFile(lua_State *L);		
	int ReadInteger( lua_State* L );
	int ReadString( lua_State* L );
	int ReadFloat( lua_State* L );
	int ReadTime( lua_State* L );

	static const char* ClassName() { return CLASS_LUAINIFILE.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUAINIFILE; }
};

}//namespace nrp