#pragma once

#include "ILuaObject.h"

namespace nrp
{
	class IniFile;
}

namespace nrp
{

class CLuaIniFile : public ILuaObject<nrp::IniFile>
{
	IniFile* _ini;
public:
	static Luna<CLuaIniFile>::RegType methods[];				//методы обертки

	CLuaIniFile(lua_State *L);	
	~CLuaIniFile();
	int ReadInteger( lua_State* L );
	int ReadString( lua_State* L );
	int ReadFloat( lua_State* L );
	int ReadTime( lua_State* L );

	static const char* ClassName();
};

}//namespace nrp