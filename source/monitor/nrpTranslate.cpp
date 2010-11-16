#include "stdafx.h"
#include "NrpTranslate.h"
#include "nrpScript.h"
#include "nrptext.h"
#include "lua.hpp"

#include <assert.h>

namespace nrp
{

namespace translate
{

NrpText GetTranslate( const NrpText& name )
{
	NrpText tmp = name;
	const char* dd = tmp;
	if( *dd == '#' )
	{
		lua_State* L = CNrpScript::Instance().GetVirtualMachine();
	
		lua_getglobal( L, dd+1);

		NrpText res = NULL;
		
		if( lua_isstring( L, -1 ) )
			res = lua_tostring( L, -1 );
		else 
			res = dd+1;

		lua_pop( L, 1 );
		return res;
	}
	else
		return name;
}

double GetNumber( const NrpText& name )
{
	NrpText tmp = name;
	const char* dd = tmp;
	if( *dd == '#')
	{
		lua_State* L = CNrpScript::Instance().GetVirtualMachine();

		lua_getglobal( L, dd+1 );

		double res = 0;
		if( lua_isnumber( L, -1 ) )
			res = lua_tonumber( L, -1 );
		else
			assert( "undefined parametr" );

		lua_pop( L, 1 );
		return res;
	}
	return
		static_cast< double >( static_cast< NrpText >( name ).ToFloat() );
}

}

}