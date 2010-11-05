#include "stdafx.h"
#include "NrpTranslate.h"
#include "nrpScript.h"
#include "StrConversation.h"
#include "lua.hpp"

#include <assert.h>

namespace nrp
{

namespace translate
{

const char* GetTranslate( const char* name )
{
	if( *name == '#' )
	{
		lua_State* L = CNrpScript::Instance().GetVirtualMachine();
	
		lua_getglobal( L, name+1);

		const char* res = NULL;
		
		if( lua_isstring( L, -1 ) )
			res = lua_tostring( L, -1 );
		else 
			res = name+1;

		lua_pop( L, 1 );
		return res;
	}
	else
		return name;
}

double GetNumber( const char* name )
{
	if( *name == '#')
	{
		lua_State* L = CNrpScript::Instance().GetVirtualMachine();

		lua_getglobal( L, name+1 );

		double res = 0;
		if( lua_isnumber( L, -1 ) )
			res = lua_tonumber( L, -1 );
		else
			assert( "undefined parametr" );

		lua_pop( L, 1 );
		return res;
	}
	return
		static_cast< double >( conv::ToFloat( name ) );
}

}

}