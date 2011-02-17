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

BEGIN_LUNA_METHODS(CLuaPlantWork)
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, Create )
	LUNA_AUTONAME_FUNCTION( CLuaPlantWork, Remove )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaPlantWork)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaPlantWork )
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "diskInDay", GetDiskInDay, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "rentPrice", GetRentPrice, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "hourPerfomance", GetHourPerfomance, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "hourPrice", GetHourPrice, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "numberMachine", GetNumberMachine, SetNumberMachine )
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "numberDay", GetNumberDay, SetNumberDay )
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "game", PureFunction, SetGame )
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "numberDisk", GetNumberDisk, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "produceType", PureFunction, SetProduceType ) 
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "diskPrice", GetDiskPrice, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "price", GetPrice, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPlantWork, "addonPrice", GetAdvPrice, PureFunction )
END_LUNA_PROPERTIES


int CLuaPlantWork::GetPrice( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushinteger( L, (*_object)[ FINALPRICE ] _;
		return 1;
	}

	lua_pushnil( L );
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

CLuaPlantWork::CLuaPlantWork( lua_State *L, bool ex ) : ILuaObject( L, CLASS_LUAPLANTWORK, ex )
{

}

int CLuaPlantWork::SetProduceType( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		CNrpDiskMachine* dm = _GetLuaObject< CNrpDiskMachine, CLuaDiskMachine >( L, -1, false );
		(*_object)[ PRODUCETYPE ] = dm;
	}

	return 0;		
}

int CLuaPlantWork::GetRentPrice( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		PNrpDiskMachine dm = (*_object)[ PRODUCETYPE ].As<PNrpDiskMachine>();
		if( dm )
		{
			int rPrice = (*dm)[ RENTPRICE ];
			lua_pushinteger( L, rPrice ); 
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;			
}

int CLuaPlantWork::GetHourPerfomance( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		PNrpDiskMachine dm = (*_object)[ PRODUCETYPE ].As<PNrpDiskMachine>();
		if( dm )
		{
			int hPerfomance = (*dm)[ DISKPERHOUR ];
			lua_pushinteger( L, hPerfomance ); 
			return 1;
		}
	}

	lua_pushnil(L);	
	return 1;		
}
 
int CLuaPlantWork::GetHourPrice( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		PNrpDiskMachine dm = (*_object)[ PRODUCETYPE ].As<PNrpDiskMachine>();
		if( dm )
		{
			int hPrice = (*dm)[PRICEPERHOUR];
			lua_pushinteger( L, hPrice ); 
			return 1;
		}
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaPlantWork::GetNumberMachine( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		int nMachine = (*_object)[ NUMBERMACHINE ];
		lua_pushinteger( L, nMachine ); 
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaPlantWork::GetNumberDay( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		int nMachine = (*_object)[NUMBERDAY];
		lua_pushinteger( L, nMachine ); 
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaPlantWork::GetNumberDisk( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		int nDisk = (*_object)[DISKNUMBER];
		lua_pushinteger( L, nDisk ); 
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaPlantWork::GetDiskPrice( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushnumber( L, (*_object)[DISKPRICE] );
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaPlantWork::SetNumberMachine( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );	
	IF_OBJECT_NOT_NULL_THEN
	{
		(*_object)[ NUMBERMACHINE ] = lua_tointeger( L, -1 );
	}

	return 0;	
}

int CLuaPlantWork::SetNumberDay( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		(*_object)[ NUMBERDAY ] = lua_tointeger( L, -1 );
	}

	return 0;	
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
	IF_OBJECT_NOT_NULL_THEN 
	{
		CNrpGame* dm = _GetLuaObject< CNrpGame, CLuaGame >( L, -1, false );
		(*_object)[ PARENT ] = dm;
	}

	return 0;		
}

int CLuaPlantWork::GetAdvPrice( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGame game = (*_object)[ PARENT ].As<PNrpGame>();
		if( game )
		{
			PNrpGameBox box = (*game)[ GBOX ].As< PNrpGameBox>();
			if( box )
			{
				lua_pushnumber( L, box->GetBoxAddonsPrice() );
				return 1;
			}
		}
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaPlantWork::GetDiskInDay( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		int nDisk = (*_object)[ DISKINDAY ];
		lua_pushinteger( L, nDisk ); 
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

const char* CLuaPlantWork::ClassName()
{
	return ( CLASS_LUAPLANTWORK );
}
}