#include "stdafx.h"
#include "LuaFunctionality.h"
#include "nrpScript.h"

ILuaFunctionality::~ILuaFunctionality( void )
{
	FunctionMap::iterator pIter = luaFunctions_.begin();
	for( ; pIter != luaFunctions_.end(); pIter++ )
		nrp::CNrpScript::Instance().ReleaseRef( pIter->second );

	luaFunctions_.clear();
}

void ILuaFunctionality::Bind( int actionType, int funcRef )
{
	assert( actionType > 0 && funcRef != -1);
	assert( luaFunctions_.find( actionType ) == luaFunctions_.end() );

	if( luaFunctions_.find( actionType ) != luaFunctions_.end() )
		return;

	luaFunctions_[ actionType ] = funcRef;
}

void ILuaFunctionality::Unbind( int actionType, int m )
{
	assert( actionType > 0 );
	assert( luaFunctions_.find( actionType ) != luaFunctions_.end() );

	FunctionMap::iterator pIter = luaFunctions_.find( actionType );
	if( pIter != luaFunctions_.end() )
		luaFunctions_.erase( pIter );
}

void ILuaFunctionality::PCall( int funcType, void* sender, void* param /*= NULL */ )
{
	try
	{
		FunctionMap::iterator pIter = luaFunctions_.find( funcType );
		if( pIter != luaFunctions_.end() )
		{
			assert( pIter->second >= 0 );
			nrp::CNrpScript::Instance().CallFunction( pIter->second, sender, param );
		}
	}
	catch(...)
	{}
}
