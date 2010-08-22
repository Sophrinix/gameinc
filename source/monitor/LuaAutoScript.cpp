#include "StdAfx.h"
#include "LuaAutoScript.h"
#include "IUser.h"
#include "NrpInvention.h"
#include "NrpApplication.h"
#include "NrpCompany.h"

#include <assert.h>

namespace nrp
{

Luna< CLuaAutoScript >::RegType CLuaAutoScript::methods[] = 
{
	LUNA_AUTONAME_FUNCTION( CLuaAutoScript, AddUserToInvention ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	{0,0}
};

CLuaAutoScript::CLuaAutoScript(lua_State *L)
{}

int CLuaAutoScript::AddUserToInvention( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaAutoScript:AddUserToInvention need userName, inventionName params" );

	const char* userName = lua_tostring( L, 2 );
	const char* inventionName = lua_tostring( L, 3 );
	assert( userName != NULL && inventionName != NULL );

	if( IUser* user = CNrpApplication::Instance().GetUser( userName ) )
		if( PNrpCompany cmp = user->GetValue<PNrpCompany>( PARENTCOMPANY ) )
			if( CNrpInvention* invention = cmp->GetInvention( inventionName ) )
			{
				invention->AddUser( user );
			}

	return 1;
}
}