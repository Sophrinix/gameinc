#include "StdAfx.h"
#include <IVideoDriver.h>
#include "LuaMiniMap.h"
#include "NrpMiniMap.h"
#include "nrpEngine.h"
#include "NrpText.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAMINIMAP( "CLuaMiniMap" );

Luna< CLuaMiniMap >::RegType CLuaMiniMap::methods[] = 
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaMiniMap ),
	/** **/
	LUNA_AUTONAME_FUNCTION( CLuaMiniMap, SetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaMiniMap, GetAngleOffset ),
	LUNA_AUTONAME_FUNCTION( CLuaMiniMap, SetOnRotateAction ),
	{0,0}
};

CLuaMiniMap::CLuaMiniMap(lua_State *L) : ILuaGuiElement(L, "CLuaMiniMap")
{}

int CLuaMiniMap::SetTexture( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetTexture need 1 parameter");

	const char* filename = lua_tostring( L, 2 );
	assert( filename != NULL );

	IF_OBJECT_NOT_NULL_THEN object_->SetTexture( CNrpEngine::Instance().GetVideoDriver()->getTexture( filename ) );

	return 1;
}

int CLuaMiniMap::GetAngleOffset( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaMiniMap::GetAngle not need any parameter");

	f32 offset = 0;

	IF_OBJECT_NOT_NULL_THEN offset = object_->GetAngleOffset();
	lua_pushnumber( L, offset );

	return 1;
}

int CLuaMiniMap::SetOnRotateAction( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetAction need 1 parameter");

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );

	IF_OBJECT_NOT_NULL_THEN object_->SetOnRotateAction( name );

	return 1;
}

const char* CLuaMiniMap::ClassName()
{
	return ( CLASS_LUAMINIMAP );
}
}//namespace nrp