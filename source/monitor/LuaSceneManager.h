#pragma once

#include "ILuaObject.h"

namespace irr
{
	namespace scene
	{
		class ISceneNode;
		class ISceneManager;
		class ITerrainSceneNode;
	}
}

namespace nrp
{
class CLuaSceneManager : public ILuaObject< irr::scene::ISceneManager >
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaSceneManager)
	static const char* ClassName();

	CLuaSceneManager(lua_State *L, bool);
	int GetActiveCamera( lua_State *vm );
	int AddTerrainSceneNode( lua_State* vm );
	int AddSkyDomeSceneNode( lua_State* vm );
	int AddCameraSceneNode( lua_State* vm );
	int AddCubeSceneNode( lua_State* vm );
	int AddTextSceneNode( lua_State* vm );
	int GetRootSceneNode( lua_State* vm );
	int GetSceneNodeByName( lua_State* vm );
	int GetSceneNodeByID( lua_State* vm );
	int GetCurrentWorldPosition( lua_State* vm );
	int GetSelectedNode( lua_State* vm );
	int SetActiveCamera( lua_State* vm );
	int AddNerpaCameraAnimator( lua_State* vm );
	int CreateTerrainTriangleSelector( lua_State* vm );
	int CreateTriangleSelectorFromBoundingBox(lua_State* vm );
	int AddToDeletionQueue( lua_State* vm );

	int CreateCollisionResponseAnimator( lua_State* vm );

	int Bind( lua_State* vm );
	int Unbind( lua_State* vm );

	int DrawProgress( lua_State* vm );
	int SetWorldSize( lua_State *vm );
	int LinkTerrain( lua_State *vm );
	int GetRayFromCursorCollisionWithTerrains( lua_State* vm );

	int SetMarkText( lua_State* L );
	int GetMarkText( lua_State* L );

	int RenderScene( lua_State* L );

	int RemoveAllNodes( lua_State* L );
	int LoadIrrlichtScene( lua_State* L );
	int SetSelectedNode( lua_State* L );
private:
	void RecalculateWorldParams_( irr::scene::ITerrainSceneNode* newTerrain );
	irr::scene::ISceneNode* GetTextSceneNode_( irr::scene::ISceneNode* ptrNode );
};

}//namespace nrp