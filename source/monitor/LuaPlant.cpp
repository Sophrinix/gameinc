#include "StdAfx.h"

#include <assert.h>
#include "LuaPlant.h"
#include "NrpReklameWork.h"
#include "NrpPlant.h"
#include "LuaReklame.h" 
#include "NrpApplication.h"
#include "LuaDiskMachine.h"
#include "LuaPlantWork.h"
#include "NrpDiskMachine.h"
#include "IniFile.h"

namespace nrp
{
CLASS_NAME CLASS_LUAPLANT( "CLuaPlant" );

BEGIN_LUNA_METHODS(CLuaPlant)
	LUNA_AUTONAME_FUNCTION( CLuaPlant, Load )
	LUNA_AUTONAME_FUNCTION( CLuaPlant, AddProduceWork )
	LUNA_AUTONAME_FUNCTION( CLuaPlant, LoadBaseReklame )
	LUNA_AUTONAME_FUNCTION( CLuaPlant, LoadReklamePrice )
	LUNA_AUTONAME_FUNCTION( CLuaPlant, AddReklameWork )
	LUNA_AUTONAME_FUNCTION( CLuaPlant, Save )
	LUNA_AUTONAME_FUNCTION( CLuaPlant, GetReklame )
	LUNA_AUTONAME_FUNCTION( CLuaPlant, GetBaseReklame )
	LUNA_AUTONAME_FUNCTION( CLuaPlant, GetBaseReklameNumber )
	LUNA_AUTONAME_FUNCTION( CLuaPlant, LoadDiskMachine )
	LUNA_AUTONAME_FUNCTION( CLuaPlant, GetDiskMachine )
	LUNA_AUTONAME_FUNCTION( CLuaPlant, GetDiskMachineNumber )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaPlant)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaPlant )
END_LUNA_PROPERTIES

CLuaPlant::CLuaPlant(lua_State *L, bool ex)	: ILuaBaseProject(L, CLASS_LUAPLANT, ex )							//конструктор
{}

int CLuaPlant::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlant::Load not need any parameter");
	
	IF_OBJECT_NOT_NULL_THEN 
	{
		_object->Load( _nrpApp[ SAVEDIR_PLANT ] );
	}

	return 1;
}

int CLuaPlant::GetDiskMachine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant:GetDiskMachine need integer parameter" );

	int dmNumber = lua_tointeger( L, 2 );
	CNrpDiskMachine* dm = NULL;
	IF_OBJECT_NOT_NULL_THEN	dm = _object->GetDiskMachine( dmNumber );

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

	NrpText dmIniFile = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpDiskMachine* dm = new CNrpDiskMachine();
		dm->Load( dmIniFile );
		_object->AddDiskMachine( dm );
	}

	return 1;		
}

int CLuaPlant::AddProduceWork( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant::AddProduceWork need CNrpPlantWork* parameter");

	CNrpPlantWork* plantWork = _GetLuaObject< CNrpPlantWork, CLuaPlantWork >( L, 2, true );
	assert( plantWork != NULL );

	IF_OBJECT_NOT_NULL_THEN _object->AddWork( plantWork );

	return 1;
}

int CLuaPlant::Save( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlant::Save not need any parameter");


	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText savedir = _nrpApp[ SAVEDIR_PLANT ];
		_object->Save( savedir );
	}

	return 1;
}

int CLuaPlant::LoadBaseReklame( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaPlant::LoadBaseReklame need reklameName, saveFilePath parameter");

	NrpText reklameName = lua_tostring( L, 2 );
	NrpText fileName = lua_tostring( L, 3 );

	bool ret = false;
	IF_OBJECT_NOT_NULL_THEN
	{
		if( _object->GetBaseReklame( reklameName ) == NULL )
		{
			CNrpReklameWork* baseReklame = new CNrpReklameWork( fileName );
			ret = _object->AddBaseReklame( baseReklame );
		}
	}

	lua_pushboolean( L, ret );
	return 1;
}

int CLuaPlant::SaveReklamePrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant::SaveReklamePrice need string parameter");

	NrpText profileName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText savedir = _nrpApp[ SAVEDIR ];
		NrpText reklamePrice = savedir+profileName+"/reklameprice.ini";

		IniFile sv( reklamePrice );
		for( int k=0; k < (int)(*_object)[ BASEREKLAMENUMBER ]; k++ )
		{
			CNrpReklameWork* rW = _object->GetBaseReklame( k );
			sv.Set( SECTION_PROPERTIES, (*rW)[ INTERNAL_NAME ], (int)(*rW)[ DAYCOST ] );
		}
	}

	return 1;
}

int CLuaPlant::LoadReklamePrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlant::LoadReklamePrice need string parameter");

	NrpText profileName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText savedir = _nrpApp[ SAVEDIR ];
		NrpText reklamePrice = savedir+profileName+"/reklameprice.ini";

		std::auto_ptr<wchar_t> buffer( new wchar_t[ 32000 ] );
		memset( buffer.get(), 0, 32000 );
		GetPrivateProfileSectionW( SECTION_PROPERTIES.ToWide(), buffer.get(), 32000, reklamePrice.ToWide() );

		NrpText readLine = buffer.get();
		while( readLine.size() )
		{
			NrpText name, valuel;
			int pos = readLine.findFirst( L'=' );
			name = readLine.subString( 0, pos );
			valuel = readLine.subString( pos + 1, 0xff );

			CNrpReklameWork* rW = _object->GetBaseReklame( name );
			if( rW != NULL )
				(*rW)[ DAYCOST ] = valuel.ToInt();

			memcpy( buffer.get(), buffer.get() + wcslen(buffer.get()) + 1, 32000 );  
			readLine = buffer.get();
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

	IF_OBJECT_NOT_NULL_THEN _object->AddReklame( reklameWork );

	return 1;
}

int CLuaPlant::GetReklame( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaPlant::GetReklame need string parameter");

	NrpText typeName = lua_tostring( L, 2 );
	NrpText game = lua_tostring( L, 3 );

	if( !typeName.size() || !game.size() ) 
		return 1;

	CNrpReklameWork* r = NULL;
	IF_OBJECT_NOT_NULL_THEN r = _object->GetReklame( typeName, game );

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
	IF_OBJECT_NOT_NULL_THEN  ret = _object->GetBaseReklame( index );
	
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