#include "StdAfx.h"
#include "LuaPlantWork.h"
#include "NrpPlantWork.h"
#include "NrpDiskMachine.h"
#include "NrpGame.h"
#include "NrpGameBox.h"
#include <assert.h>

using namespace irr;

namespace nrp
{

Luna< CLuaPlantWork >::RegType CLuaPlantWork::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaPlantWork ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, Remove ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, SetProduceType ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, GetRentPrice ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, GetHourPerfomance ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, GetHourPrice ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, GetNumberMachine ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, GetNumberDay ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, GetNumberDisk ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, SetNumberMachine ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, SetNumberDay ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, GetDiskPrice ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, GetPrice ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, SetGame ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, GetAdvPrice ),
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, GetDiskInDay ),
	{0,0}
};

int CLuaPlantWork::GetPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetPrice not need parameter" );

	int fPrice = 0;
	IF_OBJECT_NOT_NULL_THEN fPrice = object_->GetValue<int>( FINALPRICE );

	lua_pushinteger( L, fPrice ); 
	return 1;	
}

int CLuaPlantWork::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlantWork:Create need CompanyName parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );
	object_ = new CNrpPlantWork( name );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, object_ );
	Luna< CLuaPlantWork >::constructor( L );
	return 1;	
}

CLuaPlantWork::CLuaPlantWork( lua_State *L ) : ILuaObject( L, CLASS_LUAPLANTWORK )
{

}

int CLuaPlantWork::SetProduceType( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlantWork:SetPoduceType need CNrpDiskMachine* parameter" );

	CNrpDiskMachine* dm = (CNrpDiskMachine*)lua_touserdata( L, 2 );
	IF_OBJECT_NOT_NULL_THEN object_->SetValue<PNrpDiskMachine>( PRODUCETYPE, dm );

	return 1;		
}

int CLuaPlantWork::GetRentPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetRentPrice not need parameter" );

	int rPrice = 0;
	IF_OBJECT_NOT_NULL_THEN 
	{
		PNrpDiskMachine dm = object_->GetValue<PNrpDiskMachine>( PRODUCETYPE );
		if( dm )
			rPrice = dm->GetValue<int>( RENTPRICE );
	}

	lua_pushinteger( L, rPrice ); 
	return 1;			
}

int CLuaPlantWork::GetHourPerfomance( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetHourPerfomance not need parameter" );

	int hPerfomance = 0;
	IF_OBJECT_NOT_NULL_THEN 
	{
		PNrpDiskMachine dm = object_->GetValue<PNrpDiskMachine>( PRODUCETYPE );
		if( dm )
			hPerfomance = dm->GetValue<int>( DISKPERHOUR );
	}

	lua_pushinteger( L, hPerfomance ); 
	return 1;		
}
 
int CLuaPlantWork::GetHourPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetHourPrice not need parameter" );

	int hPrice = 0;
	IF_OBJECT_NOT_NULL_THEN 
	{
		PNrpDiskMachine dm = object_->GetValue<PNrpDiskMachine>( PRODUCETYPE );
		if( dm )
			hPrice = dm->GetValue<int>( PRICEPERHOUR );
	}

	lua_pushinteger( L, hPrice ); 
	return 1;	
}

int CLuaPlantWork::GetNumberMachine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetNumberMachine not need parameter" );

	int nMachine = 0;
	IF_OBJECT_NOT_NULL_THEN nMachine = object_->GetValue<int>( NUMBERMACHINE );

	lua_pushinteger( L, nMachine ); 
	return 1;	
}

int CLuaPlantWork::GetNumberDay( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetNumberDay not need parameter" );

	int nMachine = 0;
	IF_OBJECT_NOT_NULL_THEN nMachine = object_->GetValue<int>( NUMBERDAY );

	lua_pushinteger( L, nMachine ); 
	return 1;	
}

int CLuaPlantWork::GetNumberDisk( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetNumberDay not need parameter" );

	int nDisk = 0;
	IF_OBJECT_NOT_NULL_THEN nDisk = object_->GetValue<int>( DISKNUMBER );

	lua_pushinteger( L, nDisk ); 
	return 1;	
}

int CLuaPlantWork::GetDiskPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetNumberDay not need parameter" );

	float dskPrice = 0;
	IF_OBJECT_NOT_NULL_THEN dskPrice = object_->GetValue<float>( DISKPRICE);

	lua_pushnumber( L, dskPrice ); 
	return 1;	
}

int CLuaPlantWork::SetNumberMachine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlantWork:SetNumberMachine not need parameter" );

	int nMachine = lua_tointeger( L, 2 );
	IF_OBJECT_NOT_NULL_THEN object_->SetValue<int>( NUMBERMACHINE, nMachine );

	return 1;	
}

int CLuaPlantWork::SetNumberDay( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlantWork:SetNumberDay not need parameter" );

	int nDay = lua_tointeger( L, 2 );
	IF_OBJECT_NOT_NULL_THEN object_->SetValue<int>( NUMBERDAY, nDay );

	return 1;	
}

int CLuaPlantWork::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:Remove not need any parameter" );

	delete object_;
	object_ = NULL;

	return 1;	
}

int CLuaPlantWork::SetGame( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlantWork:SetPoduceType need CNrpDiskMachine* parameter" );

	CNrpGame* dm = (CNrpGame*)lua_touserdata( L, 2 );
	IF_OBJECT_NOT_NULL_THEN object_->SetValue<PNrpGame>( PARENT, dm );

	return 1;		
}

int CLuaPlantWork::GetAdvPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetNumberDay not need parameter" );

	float addonPrice = 0;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGame game = object_->GetValue<PNrpGame>( PARENT );
		if( game )
		{
			PNrpGameBox box = game->GetValue<PNrpGameBox>( GBOX );
			if( box )
				addonPrice = box->GetBoxAddonsPrice();
		}
	}

	lua_pushnumber( L, addonPrice ); 
	return 1;	
}

int CLuaPlantWork::GetDiskInDay( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetDiskInDay not need parameter" );

	int nDisk = 0;
	IF_OBJECT_NOT_NULL_THEN nDisk = object_->GetValue<int>( DISKINDAY );

	lua_pushinteger( L, nDisk ); 
	return 1;	
}
}