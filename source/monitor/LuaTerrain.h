#pragma once

#include "ILuaSceneNode.h"

namespace irr
{
	namespace scene
	{
		class ITerrainSceneNode;
	}
};

namespace nrp
{
class CLuaTerrain : public ILuaSceneNode< irr::scene::ITerrainSceneNode >
{
public:
	static Luna<CLuaTerrain>::RegType methods[];
	static const char* ClassName();

	// Initialize the pointer
	CLuaTerrain(lua_State *L);

	int ScaleTexture( lua_State *L );
	int SetMaterialType( lua_State *L );
	int GetSideProjSize( lua_State *L );
	
	~CLuaTerrain();
};

}//namespace nrp