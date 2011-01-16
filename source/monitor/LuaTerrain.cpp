#include "StdAfx.h"
#include "LuaTerrain.h"
#include <lua.hpp>
#include <irrlicht.h>

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUATERRAIN( "CLuaTerrain" );

BEGIN_LUNA_METHODS(CLuaTerrain)
	LUNA_ILUASCENENODE_HEADER( CLuaTerrain )
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaTerrain, ScaleTexture )
	LUNA_AUTONAME_FUNCTION( CLuaTerrain, SetMaterialType )
	LUNA_AUTONAME_FUNCTION( CLuaTerrain, GetSideProjSize )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaTerrain)
END_LUNA_PROPERTIES

CLuaTerrain::CLuaTerrain(lua_State *L, bool ex) : ILuaSceneNode(L, CLASS_LUATERRAIN, ex)
{}

CLuaTerrain::~CLuaTerrain()
{
}


/************************************************************************/
/* ѕолучение размеров( ширины и длины ) поверхности                     */
/************************************************************************/
int CLuaTerrain::GetSideProjSize( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTerrain::GetSideProjSize not need any parameter");

	core::aabbox3df box;
	IF_OBJECT_NOT_NULL_THEN box = _object->getBoundingBox();
	f32 sidex = box.MaxEdge.X - box.MinEdge.X; 
	f32 sidez = box.MaxEdge.Z - box.MinEdge.Z; 
	
	lua_tointeger( L, (s32)sidex );
	lua_tointeger( L, (s32)sidez );

	return 2;
}

int CLuaTerrain::ScaleTexture( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck( L, argc == 3, 3, "Function CLuaTerrain::scaleTexture need two parameter" );

	float scalex = (float)lua_tonumber( L, 2 );
	float scaley = (float)lua_tonumber( L, 3 );

	IF_OBJECT_NOT_NULL_THEN _object->scaleTexture( scalex, scaley );

	return 1;
}

int CLuaTerrain::SetMaterialType( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck( L, argc == 2, 2, "Function CLuaTerrain::setMaterialFlag need one parameter" );

	int matr_number = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->setMaterialType( (irr::video::E_MATERIAL_TYPE)matr_number );

	return 1;
}

const char* CLuaTerrain::ClassName()
{
	return ( CLASS_LUATERRAIN );
}	
}//namespace nrp