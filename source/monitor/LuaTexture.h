#pragma once

#include "ILuaObject.h"

namespace irr
{
	namespace video
	{
		class ITexture;
	}
}

namespace nrp
{
CLASS_NAME CLASS_LUATEXTURE( "CLuaTexture" );

class CLuaTexture : public ILuaObject<irr::video::ITexture>
{
public:
	static Luna<CLuaTexture>::RegType methods[];

	CLuaTexture(lua_State *L);
	int GetSize( lua_State *L );
	int GetWidth( lua_State *L );
	int GetHeight( lua_State *L );
	int Drop( lua_State *L );

	static const char* ClassName() { return CLASS_LUATEXTURE.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUATEXTURE; }
};

}//namespace nrp