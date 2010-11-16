#include "StdAfx.h"
#include "LuaCamera.h"

#include <ICameraSceneNode.h>
#include "nrpCameraAnimator.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUACAMERA( "CLuaCamera" );

Luna< CLuaCamera >::RegType CLuaCamera::methods[] = 
{
	LUNA_ILUASCENENODE_HEADER( CLuaCamera ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaCamera, GetZoom ),
	LUNA_AUTONAME_FUNCTION( CLuaCamera, SetZoom ),
	LUNA_AUTONAME_FUNCTION( CLuaCamera, SetRotate ),
	LUNA_AUTONAME_FUNCTION( CLuaCamera, GetRotate ),
	LUNA_AUTONAME_FUNCTION( CLuaCamera, AddAnimator ),
	LUNA_AUTONAME_FUNCTION( CLuaCamera, SetFarValue ),
	LUNA_AUTONAME_FUNCTION( CLuaCamera, SetPosition ),
	LUNA_AUTONAME_FUNCTION( CLuaCamera, SetTarget ),
	LUNA_AUTONAME_FUNCTION( CLuaCamera, AddTerrain ),
	{0,0}
};

void CLuaCamera::FindRtsAnimator_()
{
	anim_ = NULL;

	core::list< scene::ISceneNodeAnimator* >  anims = object_->getAnimators();
	core::list< scene::ISceneNodeAnimator* >::Iterator animator = anims.begin();

	for(; animator != anims.end(); animator++ )
		if( (*animator)->getType() == scene::ESNAT_CAMERA_RTS )
		{
			anim_ = dynamic_cast< scene::CNrpCameraAnimator* >( *animator );
			break;
		}
}

CLuaCamera::CLuaCamera(lua_State *L) : ILuaSceneNode( L, CLASS_LUACAMERA )
{
	IF_OBJECT_NOT_NULL_THEN FindRtsAnimator_();
}

int CLuaCamera::SetObject(lua_State *L)
{
	ILuaObject::SetObject(L);

	IF_OBJECT_NOT_NULL_THEN FindRtsAnimator_();

	return 1;
}

int CLuaCamera::GetZoom( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaCamera::GetZoom not need any parameter");

	float result = 0;
	
	IF_OBJECT_NOT_NULL_THEN
	{
		if( anim_ ) 
			result = anim_->getZoom();
	}

	lua_pushnumber( L, result );

	return 1;
}

int CLuaCamera::SetZoom( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCamera::SetZoom need float parameter");

	float amount = (float)lua_tonumber( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( anim_ )
			anim_->setZoom( amount );
	}

	return 1;
}

int CLuaCamera::SetRotate( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaCamera::SetRotate need 3 parameter");

	core::vector3df amount;
	amount.X = (float)lua_tonumber( L, 2 );
	amount.Y = (float)lua_tonumber( L, 3 );
	amount.Z = (float)lua_tonumber( L, 4 );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( anim_ )
			anim_->setRotate( amount );
	}

	return 1;
}

int CLuaCamera::GetRotate( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaCamera::GetRotate not need any parameter");

	core::vector3df rotate;

	IF_OBJECT_NOT_NULL_THEN
	{
		if( anim_ )
			rotate = anim_->getRotate();
	}
	lua_pushnumber( L, rotate.X );
	lua_pushnumber( L, rotate.Y );
	lua_pushnumber( L, rotate.Z );

	return 3;
}


int CLuaCamera::AddAnimator( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCamera::AddAnimator need one parameter");

	scene::ISceneNodeAnimator*  anim = (scene::ISceneNodeAnimator*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->addAnimator( anim );

	return 1;
}

int CLuaCamera::SetFarValue( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCamera::SetFarValue need float parameter");

	float amount = (float)lua_tonumber( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->setFarValue( amount );

	return 1;
}

int CLuaCamera::SetPosition( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaCamera::SetPosition need 4 parameter");

	core::vector3df pos;
	pos.X = (float)lua_tonumber( L, 2 );
	pos.Y = (float)lua_tonumber( L, 3 );
	pos.Z = (float)lua_tonumber( L, 4 );

	IF_OBJECT_NOT_NULL_THEN	object_->setPosition( pos );

	return 1;
}

int CLuaCamera::SetTarget( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaCamera::SetPosition need 4 parameter");

	core::vector3df pos;
	pos.X = (float)lua_tonumber( L, 2 );
	pos.Y = (float)lua_tonumber( L, 3 );
	pos.Z = (float)lua_tonumber( L, 4 );

	IF_OBJECT_NOT_NULL_THEN	object_->setTarget( pos );

	return 1;
}

int CLuaCamera::AddTerrain( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaCamera::SetRotate need ITerrainSceneNode parameter");

	scene::ITerrainSceneNode* terrain = (scene::ITerrainSceneNode*)lua_touserdata( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( anim_ )
			anim_->AddTerrainToResponse( terrain );
	}

	return 1;
}

const char* CLuaCamera::ClassName()
{
	return ( CLASS_LUACAMERA );
}
}