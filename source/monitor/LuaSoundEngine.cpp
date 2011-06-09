#include "StdAfx.h"
#include "LuaSoundEngine.h"

namespace nrp
{
CLASS_NAME CLASS_CLUASOUNDENGINE( "CLuaSoundEngine" );

BEGIN_LUNA_METHODS(CLuaSoundEngine)
	LUNA_AUTONAME_FUNCTION( CLuaSoundEngine, Play )
	LUNA_AUTONAME_FUNCTION( CLuaSoundEngine, PlayBackground )
	LUNA_AUTONAME_FUNCTION( CLuaSoundEngine, AddResourceDirectory )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaSoundEngine)
END_LUNA_PROPERTIES

CLuaSoundEngine::CLuaSoundEngine( lua_State *L, bool ex ) : ILuaProject(L, CLASS_CLUASOUNDENGINE, ex )
{

}

CLuaSoundEngine::~CLuaSoundEngine(void)
{
}

const char* CLuaSoundEngine::ClassName()
{
	return CLASS_CLUASOUNDENGINE;
}

int CLuaSoundEngine::PlayBackground( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaSoundEngine:PlayBackground need path to file" );

	NrpText pathToFile( lua_tostring( L, 2 ) );

	IF_OBJECT_NOT_NULL_THEN 
	{
		_object->Play( "__background", pathToFile, true );
	}

	return 0;
}

int CLuaSoundEngine::Play( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaSoundEngine:Play need path to file" );

	NrpText pathToFile( lua_tostring( L, 2 ) );
	IF_OBJECT_NOT_NULL_THEN 
	{
		_object->Play( "", pathToFile, false );
	}

	return 0;
}

int CLuaSoundEngine::AddResourceDirectory( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaSoundEngine:AddSoundsDirectory need path to file" );

	NrpText pathToDir( lua_tostring( L, 2 ) );
	IF_OBJECT_NOT_NULL_THEN 
	{
		_object->AddSoundsDirectory( pathToDir );
	}

	return 0;
}

}