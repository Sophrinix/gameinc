#include "StdAfx.h"

#include <assert.h>
#include "LuaPlant.h"
#include "NrpReklameWork.h"
#include "NrpPlant.h"
#include "LuaReklame.h" 
#include "NrpApplication.h"
#include "LuaDiskMachine.h"
#include "NrpDiskMachine.h"
#include "IniFile.h"

namespace nrp
{
CLASS_NAME CLASS_LUAPLANT( "CLuaPlant" );

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
	LUNA_AUTONAME_FUNCTION( CLuaPlant, GetBaseReklame ),
	LUNA_AUTONAME_FUNCTION( CLuaPlant, GetBaseReklameNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaPlant, LoadDiskMachine ),
	LUNA_AUTONAME_FUNCTION( CLuaPlant, GetDiskMachine ),
	LUNA_AUTONAME_FUNCTION( CLuaPlant, GetDiskMachineNumber ),
	{0,0}
};

CLuaPlant::CLuaPlant(lua_State *L)	: ILuaBaseProject(L, CLASS_LUAPLANT )							//конструктор
{}

int CLuaPlant::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlant::Load not need any parameter");
	
	IF_OBJECT_NOT_NULL_THEN 
	{
		object_->Load( CNrpApplication::Instance().GetString( SAVEDIR_PLANT ) );
	}

	return 1;
}

int CLuaPlant::GetDiskMachine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant:GetDiskMachine need integer parameter" );

	int dmNumber = lua_tointeger( L, 2 );
	CNrpDiskMachine* dm = NULL;
	IF_OBJECT_NOT_NULL_THEN	dm = object_->GetDiskMachine( dmNumber );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, dm );
	Luna< CLuaDiskMachine >::constructor( L );

	return 1;	
}

int CLuaPlant::GetDiskMachineNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetDiskMachineNumber", DISKMACHINENUMBER, 0 ) );
	return 1;
}

int CLuaPlant::LoadDiskMachine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant:LoadDiskMachine need string parameter" );

	const char* dmIniFile = lua_tostring( L, 2 );
	assert( dmIniFile != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpDiskMachine* dm = new CNrpDiskMachine();
		dm->Load( dmIniFile );
		object_->AddDiskMachine( dm );
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
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlant::Save not need any parameter");


	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText savedir = CNrpApplication::Instance().GetString( SAVEDIR_PLANT );
		object_->Save( savedir );
	}

	return 1;
}

int CLuaPlant::LoadBaseReklame( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaPlant::LoadBaseReklame need reklameName, saveFilePath parameter");

	const char* reklameName = lua_tostring( L, 2 );
	const char* fileName = lua_tostring( L, 3 );
	assert( fileName != NULL );

	bool ret = false;
	IF_OBJECT_NOT_NULL_THEN
	{
		if( object_->GetBaseReklame( reklameName ) == NULL )
		{
			CNrpReklameWork* baseReklame = new CNrpReklameWork( fileName );
			ret = object_->AddBaseReklame( baseReklame );
		}
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
		NrpText savedir = CNrpApplication::Instance().GetString( SAVEDIR );
		NrpText reklamePrice = savedir+profileName+"/reklameprice.ini";

		IniFile sv( reklamePrice );
		for( int k=0; k < object_->GetValue<int>( BASEREKLAMENUMBER ); k++ )
		{
			CNrpReklameWork* rW = object_->GetBaseReklame( k );
			sv.Set( SECTION_PROPERTIES, rW->GetString( TECHTYPE ), rW->GetValue<int>( DAYCOST ) );
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
		NrpText savedir = CNrpApplication::Instance().GetString( SAVEDIR );
		NrpText reklamePrice = savedir+profileName+"/reklameprice.ini";

		wchar_t buffer[ 32000 ];
		memset( buffer, 0, 32000 );
		GetPrivateProfileSectionW( SECTION_PROPERTIES.c_str(), buffer, 32000, reklamePrice.c_str() );

		NrpText readLine = buffer;
		while( readLine.size() )
		{
			NrpText name, valuel;
			int pos = readLine.findFirst( L'=' );
			name = readLine.subString( 0, pos );
			valuel = readLine.subString( pos + 1, 0xff );

			CNrpReklameWork* rW = object_->GetBaseReklame( name );
			if( rW != NULL )
				rW->SetValue<int>( DAYCOST, valuel.ToInt() );

			memcpy( buffer, buffer + wcslen(buffer) + 1, 32000 );  
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

int CLuaPlant::GetBaseReklameNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetBaseReklameNumber", BASEREKLAMENUMBER, 0 ) );
	return 1;
}

int CLuaPlant::GetBaseReklame( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant::GetBaseReklame need int parameter");

	int index = lua_tointeger( L, 2 );
	assert( index >= 0 );

	CNrpReklameWork* ret = NULL;
	IF_OBJECT_NOT_NULL_THEN  ret = object_->GetBaseReklame( index );
	
	lua_pop( L, argc );
	lua_pushlightuserdata( L, ret );
	Luna< CLuaReklame >::constructor( L );
	return 1;
}

const char* CLuaPlant::ClassName()
{
	return ( CLASS_LUAPLANT );
}

}//namespace nrp