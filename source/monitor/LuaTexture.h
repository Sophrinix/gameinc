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
	DEFINE_PROPERTIES_AND_METHODS(CLuaTexture)
	static const char* ClassName();

	CLuaTexture(lua_State *L, bool );
	int GetSize( lua_State *L );
	int GetWidth( lua_State *L );
	int GetHeight( lua_State *L );
	int Drop( lua_State *L );
};

}//namespace nrp