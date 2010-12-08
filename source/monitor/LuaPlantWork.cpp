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
CLASS_NAME CLASS_LUAPLANTWORK("CLuaPlantWork");

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
	IF_OBJECT_NOT_NULL_THEN fPrice = (*_object)[ FINALPRICE ];

	lua_pushinteger( L, fPrice ); 
	return 1;	
}

int CLuaPlantWork::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlantWork:Create need CompanyName parameter" );

	NrpText name = lua_tostring( L, 2 );

	_object = new CNrpPlantWork( name );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, _object );
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
	IF_OBJECT_NOT_NULL_THEN (*_object)[ PRODUCETYPE ] = dm;

	return 1;		
}

int CLuaPlantWork::GetRentPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetRentPrice not need parameter" );

	int rPrice = 0;
	IF_OBJECT_NOT_NULL_THEN 
	{
		PNrpDiskMachine dm = (*_object)[ PRODUCETYPE ].As<PNrpDiskMachine>();
		if( dm )
			rPrice = (*dm)[ RENTPRICE ];
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
		PNrpDiskMachine dm = (*_object)[ PRODUCETYPE ].As<PNrpDiskMachine>();
		if( dm )
			hPerfomance = (*dm)[ DISKPERHOUR ];
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
		PNrpDiskMachine dm = (*_object)[ PRODUCETYPE ].As<PNrpDiskMachine>();
		if( dm )
			hPrice = (*dm)[PRICEPERHOUR];
	}

	lua_pushinteger( L, hPrice ); 
	return 1;	
}

int CLuaPlantWork::GetNumberMachine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetNumberMachine not need parameter" );

	int nMachine = 0;
	IF_OBJECT_NOT_NULL_THEN nMachine = (*_object)[ NUMBERMACHINE ];

	lua_pushinteger( L, nMachine ); 
	return 1;	
}

int CLuaPlantWork::GetNumberDay( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetNumberDay not need parameter" );

	int nMachine = 0;
	IF_OBJECT_NOT_NULL_THEN nMachine = (*_object)[NUMBERDAY];

	lua_pushinteger( L, nMachine ); 
	return 1;	
}

int CLuaPlantWork::GetNumberDisk( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetNumberDay not need parameter" );

	int nDisk = 0;
	IF_OBJECT_NOT_NULL_THEN nDisk = (*_object)[DISKNUMBER];

	lua_pushinteger( L, nDisk ); 
	return 1;	
}

int CLuaPlantWork::GetDiskPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetNumberDay not need parameter" );

	float dskPrice = 0;
	IF_OBJECT_NOT_NULL_THEN dskPrice = (*_object)[DISKPRICE];

	lua_pushnumber( L, dskPrice ); 
	return 1;	
}

int CLuaPlantWork::SetNumberMachine( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlantWork:SetNumberMachine not need parameter" );

	int nMachine = lua_tointeger( L, 2 );
	IF_OBJECT_NOT_NULL_THEN (*_object)[ NUMBERMACHINE ] = nMachine;

	return 1;	
}

int CLuaPlantWork::SetNumberDay( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlantWork:SetNumberDay not need parameter" );

	int nDay = lua_tointeger( L, 2 );
	IF_OBJECT_NOT_NULL_THEN (*_object)[ NUMBERDAY ] = nDay;

	return 1;	
}

int CLuaPlantWork::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:Remove not need any parameter" );

	delete _object;
	_object = NULL;

	return 1;	
}

int CLuaPlantWork::SetGame( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlantWork:SetPoduceType need CNrpDiskMachine* parameter" );

	CNrpGame* dm = (CNrpGame*)lua_touserdata( L, 2 );
	IF_OBJECT_NOT_NULL_THEN (*_object)[ PARENT ] = dm;

	return 1;		
}

int CLuaPlantWork::GetAdvPrice( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPlantWork:GetNumberDay not need parameter" );

	float addonPrice = 0;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGame game = (*_object)[ PARENT ].As<PNrpGame>();
		if( game )
		{
			PNrpGameBox box = (*game)[ GBOX ].As< PNrpGameBox>();
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
	IF_OBJECT_NOT_NULL_THEN nDisk = (*_object)[ DISKINDAY ];

	lua_pushinteger( L, nDisk ); 
	return 1;	
}

const char* CLuaPlantWork::ClassName()
{
	return ( CLASS_LUAPLANTWORK );
}
}