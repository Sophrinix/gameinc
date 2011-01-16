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

BEGIN_LUNA_METHODS(CLuaMiniMap)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaMiniMap )
	/** **/
	LUNA_AUTONAME_FUNCTION( CLuaMiniMap, SetTexture )
	LUNA_AUTONAME_FUNCTION( CLuaMiniMap, GetAngleOffset )
	LUNA_AUTONAME_FUNCTION( CLuaMiniMap, SetOnRotateAction )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaMiniMap)
END_LUNA_PROPERTIES

CLuaMiniMap::CLuaMiniMap(lua_State *L, bool ex) : ILuaGuiElement(L, CLASS_LUAMINIMAP, ex)
{}

int CLuaMiniMap::SetTexture( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetTexture need 1 parameter");

	NrpText filename = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->SetTexture( _nrpEngine.GetVideoDriver()->getTexture( filename ) );

	return 1;
}

int CLuaMiniMap::GetAngleOffset( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaMiniMap::GetAngle not need any parameter");

	f32 offset = 0;

	IF_OBJECT_NOT_NULL_THEN offset = _object->GetAngleOffset();
	lua_pushnumber( L, offset );

	return 1;
}

int CLuaMiniMap::SetOnRotateAction( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetAction need 1 parameter");

	NrpText name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->SetOnRotateAction( name );

	return 1;
}

const char* CLuaMiniMap::ClassName()
{
	return ( CLASS_LUAMINIMAP );
}
}//namespace nrp