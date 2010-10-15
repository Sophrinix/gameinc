#include "StdAfx.h"

#include <assert.h>
#include "LuaPlant.h"
#include "NrpReklameWork.h"
#include "NrpPlant.h"
#include "LuaReklame.h" 
#include "NrpApplication.h"

namespace nrp
{

Luna< CLuaPlant >::RegType CLuaPlant::methods[] =			//реализуемы методы
{
	LUNA_ILUAOBJECT_HEADER( CLuaPlant ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaPlant, Load ),
	LUNA_AUTONAME_FUNCTION( CLuaPlant, AddProduceWork ),
	LUNA_AUTONAME_FUNCTION( CLuaPlant, LoadBaseReklame ),
	LUNA_AUTONAME_FUNCTION( CLuaPlant, LoadReklamePrice ),
	LUNA_AUTONAME_FUNCTION( CLuaPlant, AddReklameWork ),
	LUNA_AUTONAME_FUNCTION( CLuaPlant, Save ),
	LUNA_AUTONAME_FUNCTION( CLuaPlant, GetReklame ),
	{0,0}
};

CLuaPlant::CLuaPlant(lua_State *L)	: ILuaObject(L, CLASS_LUAPLANT )							//конструктор
{}

int CLuaPlant::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant::Load need string parameter");

	const char* fileName = lua_tostring( L, 2 );
	assert( fileName != NULL );

	
	IF_OBJECT_NOT_NULL_THEN 
	{
		std::string savedir = CNrpApplication::Instance().GetValue<std::string>( SAVEDIR );
		object_->Load( SECTION_OPTIONS, savedir+fileName+"/" );
	}

	return 1;
}

int CLuaPlant::AddProduceWork( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant::AddProduceWork need CNrpPlantWork* parameter");

	CNrpPlantWork* plantWork = (CNrpPlantWork*)lua_touserdata( L, 2 );
	assert( plantWork != NULL );

	IF_OBJECT_NOT_NULL_THEN object_->AddWork( plantWork );

	return 1;
}

int CLuaPlant::Save( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant::Save need string parameter");

	const char* fileName = lua_tostring( L, 2 );
	assert( fileName != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		std::string savedir = CNrpApplication::Instance().GetValue<std::string>( SAVEDIR );
		object_->Save( SECTION_OPTIONS, savedir+fileName+"/" );
	}

	return 1;
}

int CLuaPlant::LoadBaseReklame( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant::LoadBaseReklame need string parameter");

	const char* fileName = lua_tostring( L, 2 );
	assert( fileName != NULL );

	bool ret = false;
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpReklameWork* baseReklame = new CNrpReklameWork( "base", "" );
		baseReklame->Load( SECTION_PROPERTIES, fileName );
		ret = object_->AddBaseReklame( baseReklame );
	}

	lua_pushboolean( L, ret );
	return 1;
}

int CLuaPlant::SaveReklamePrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant::SaveReklamePrice need string parameter");

	const char* profileName = lua_tostring( L, 2 );
	assert( profileName != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		std::string savedir = CNrpApplication::Instance().GetValue<std::string>( SAVEDIR );
		std::string reklamePrice = savedir+profileName+"/reklameprice.ini";
		for( int k=0; k < object_->GetValue<int>( BASEREKLAMENUMBER ); k++ )
		{
			CNrpReklameWork* rW = object_->GetBaseReklame( k );
			IniFile::Write( SECTION_PROPERTIES, rW->GetValue<std::string>( TECHTYPE ), rW->GetValue<int>( DAYCOST ), reklamePrice );
		}
	}

	return 1;
}

int CLuaPlant::LoadReklamePrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant::LoadReklamePrice need string parameter");

	const char* profileName = lua_tostring( L, 2 );
	assert( profileName != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		std::string savedir = CNrpApplication::Instance().GetValue<std::string>( SAVEDIR );
		std::string reklamePrice = savedir+profileName+"/reklameprice.ini";

		char buffer[ 32000 ];
		memset( buffer, 0, 32000 );
		GetPrivateProfileSection( SECTION_PROPERTIES.c_str(), buffer, 32000, reklamePrice.c_str() );

		std::string readLine = buffer;
		while( readLine != "" )
		{
			std::string name, valuel;
			name = readLine.substr( 0, readLine.find( '=' ) );
			valuel = readLine.substr( readLine.find( '=' ) + 1, 0xff );

			CNrpReklameWork* rW = object_->GetBaseReklame( name );
			if( rW != NULL )
				rW->SetValue<int>( DAYCOST,StrToInt( valuel.c_str() ) );

			memcpy( buffer, buffer + strlen(buffer) + 1, 32000 );  
			readLine = buffer;
		}
	}
	return 1;
}

int CLuaPlant::AddReklameWork( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant::AddReklameWork need CNrpReklameWork* parameter");

	CNrpReklameWork* reklameWork = (CNrpReklameWork*)lua_touserdata( L, 2 );
	assert( reklameWork != NULL );

	IF_OBJECT_NOT_NULL_THEN object_->AddReklame( reklameWork );

	return 1;
}

int CLuaPlant::GetReklame( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaPlant::GetReklame need string parameter");

	const char* typeName = lua_tostring( L, 2 );
	const char* game = lua_tostring( L, 3 );
	assert( typeName && game );

	if( !typeName || !game ) 
		return 1;

	CNrpReklameWork* r = NULL;
	IF_OBJECT_NOT_NULL_THEN r = object_->GetReklame( typeName, game );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, r );
	Luna< CLuaReklame >::constructor( L );
	return 1;
}

}//namespace nrp