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
CLASS_NAME CLASS_LUATERRAIN( "CLuaTerrain" );

class CLuaTerrain : public ILuaSceneNode< irr::scene::ITerrainSceneNode >
{
public:
	static Luna<CLuaTerrain>::RegType methods[];

	// Initialize the pointer
	CLuaTerrain(lua_State *L);

	int ScaleTexture( lua_State *L );
	int SetMaterialType( lua_State *L );
	int GetSideProjSize( lua_State *L );

	static const char* ClassName() { return CLASS_LUATERRAIN.c_str(); }
	
	~CLuaTerrain();
};

}//namespace nrp