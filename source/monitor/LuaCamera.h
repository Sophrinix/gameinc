#pragma once

#include "ILuaSceneNode.h"

namespace irr
{
	namespace scene
	{
		class ICameraSceneNode;
		class CNrpCameraAnimator;
	}
}

namespace nrp
{
CLASS_NAME CLASS_LUACAMERA( "CLuaCamera" );

class CLuaCamera : public ILuaSceneNode< irr::scene::ICameraSceneNode >
{
public:
	static Luna<CLuaCamera>::RegType methods[];

	CLuaCamera(lua_State *L);

	int GetZoom( lua_State *L );
	int SetZoom( lua_State *L );
	int SetRotate( lua_State *L );
	int AddAnimator( lua_State* L );
	int SetFarValue( lua_State* L );
	int SetPosition( lua_State* L );
	int SetTarget( lua_State* L );
	int AddTerrain( lua_State* L );
	int GetRotate( lua_State *L );
	int SetObject(lua_State *L);

	static const char* ClassName() { return CLASS_LUACAMERA.c_str(); }

private:
	irr::scene::CNrpCameraAnimator* anim_;
	void FindRtsAnimator_();
};

}//namespace nrp