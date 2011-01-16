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
	//методы обертки
	DEFINE_PROPERTIES_AND_METHODS(CLuaIniFile)

	CLuaIniFile(lua_State *L, bool ex);	
	~CLuaIniFile();
	int ReadInteger( lua_State* L );
	int ReadString( lua_State* L );
	int ReadFloat( lua_State* L );
	int ReadTime( lua_State* L );

	static const char* ClassName();
};

}//namespace nrp