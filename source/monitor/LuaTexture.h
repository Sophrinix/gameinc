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
class CLuaTexture : public ILuaObject<irr::video::ITexture>
{
public:
	static Luna<CLuaTexture>::RegType methods[];
	static const char* ClassName();

	CLuaTexture(lua_State *L);
	int GetSize( lua_State *L );
	int GetWidth( lua_State *L );
	int GetHeight( lua_State *L );
	int Drop( lua_State *L );
};

}//namespace nrp