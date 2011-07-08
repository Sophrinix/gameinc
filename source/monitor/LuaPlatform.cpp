#include "StdAfx.h"
#include "LuaPlatform.h"
#include "NrpPlatform.h"
#include "NrpTechnology.h"
#include "LuaTechnology.h"
#include "NrpHistory.h"
#include "NrpApplication.h"
#include "NrpGameMarket.h"

namespace nrp
{
CLASS_NAME CLASS_LUAPLATFORM( "CLuaPlatform" );

BEGIN_LUNA_METHODS(CLuaPlatform)
	LUNA_AUTONAME_FUNCTION( CLuaPlatform, Load )
	LUNA_AUTONAME_FUNCTION( CLuaPlatform, Create )
	LUNA_AUTONAME_FUNCTION( CLuaPlatform, IsMyTech )
    LUNA_AUTONAME_FUNCTION( CLuaPlatform, CreateRandomSales )
    LUNA_AUTONAME_FUNCTION( CLuaPlatform, CreateSalesHistory )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaPlatform)
	LUNA_ILUAPROJECT_PROPERTIES(CLuaPlatform)
	LUNA_AUTONAME_PROPERTY( CLuaPlatform, "cpu", GetCpu, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPlatform, "ram", GetRam, PureFunction )
    LUNA_AUTONAME_PROPERTY( CLuaPlatform, "lastMonthSales", GetLastMonthSales, PureFunction )
    LUNA_AUTONAME_PROPERTY( CLuaPlatform, "price", GetPrice, SetPrice )
    LUNA_AUTONAME_PROPERTY( CLuaPlatform, "lastMonthProfit", GetLastMonthProfit, PureFunction )
    LUNA_AUTONAME_PROPERTY( CLuaPlatform, "allTimeSales", GetAllTimeSales, PureFunction )
    LUNA_AUTONAME_PROPERTY( CLuaPlatform, "allTimeProfit", GetAllTimeProfit, PureFunction )
END_LUNA_PROPERTIES

CLuaPlatform::CLuaPlatform(lua_State* L, bool ex) : ILuaProject<CNrpPlatform>(L, CLASS_LUAPLATFORM, ex )
{
	
}

int CLuaPlatform::GetPrice( lua_State* L )
{
    IF_OBJECT_NOT_NULL_THEN
    {
        lua_pushinteger( L, (*_object)[ PRICE ] );
        return 1;
    }

    lua_pushnil( L );
    return 1;		
}

int CLuaPlatform::GetAllTimeProfit( lua_State* L )
{
    IF_OBJECT_NOT_NULL_THEN 
    {
        if( CNrpHistory* history = (*_object)[ HISTORY ].As<CNrpHistory*>() )
        {
            lua_pushinteger( L, history->GetSummFor( BALANCE, _nrpApp[ CURRENTTIME ].As<NrpTime>() ) );
            return 1;
        }
    }

    lua_pushnil( L );
    return 1;		
}

int CLuaPlatform::GetLastMonthProfit( lua_State* L )
{
    IF_OBJECT_NOT_NULL_THEN
    {
        int sales = 0;
        if( CNrpHistory* history = (*_object)[ HISTORY ].As<CNrpHistory*>() )
        {
            if( CNrpHistoryStep* step = history->GetLast() )
            {
                lua_pushinteger( L, (*step)[ BALANCE ] );
                return 1;
            }
        }
    }

    lua_pushnil( L );
    return 1;
}

int CLuaPlatform::GetLastMonthSales( lua_State* L )
{
    IF_OBJECT_NOT_NULL_THEN
    {
        if(CNrpHistory* history = (*_object)[ HISTORY ].As<CNrpHistory*>() )
        {
            int sales = 0;
            if( CNrpHistoryStep* step = history->GetLast() )
                sales = (*step)[ COPYSELL ];

            lua_pushinteger( L, sales );
            return 1;
        }
    }

    lua_pushnil( L );
    return 1;
}

int CLuaPlatform::SetPrice( lua_State* L )
{
    assert( lua_isnumber( L, -1 ) );
    int newPrice = lua_tointeger( L, -1 );

    IF_OBJECT_NOT_NULL_THEN
    {
       (*_object)[ PRICE ] = newPrice;
    }

    return 0;		
}

int CLuaPlatform::GetAllTimeSales( lua_State* L )
{
    IF_OBJECT_NOT_NULL_THEN 
    {
        if( CNrpHistory* history = (*_object)[ HISTORY ].As<CNrpHistory*>() )
        {
            lua_pushinteger( L, history->GetSummFor( COPYSELL, _nrpApp[ CURRENTTIME ].As<NrpTime>() ) );
            return 1;
        }
    }

    lua_pushnil( L );
    return 1;		
}

int CLuaPlatform::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlatform:Create need platformName parameter" );

	NrpText typeName = lua_tostring( L, 2 );

	_object = new CNrpPlatform( typeName );

	//lua_pop( L, argc );
	lua_pushlightuserdata(L, _object );
	Luna< CLuaPlatform >::constructor( L );

	return 1;
}

int CLuaPlatform::CreateSalesHistory( lua_State* L )
{
    int argc = lua_gettop(L);
    luaL_argcheck(L, argc == 1, 1, "Function CLuaPlatform:CreateSalesHistory not need parameter" );

    IF_OBJECT_NOT_NULL_THEN 
    {
        int fMonth = NrpTime( (*_object)[ STARTDATE ] ).GetMonthToDate( _nrpApp[ CURRENTTIME ].As<NrpTime>() );//полное количество месяцев жизни игры
        int price = (*_object)[ PRICE ];
        CNrpHistory* hs = (*_object)[ HISTORY ].As<CNrpHistory*>();
        assert( hs && price >= 100 );

        if( !hs )
        {
            assert( "no history for platform" );
            return 0;
        }
        
        int lastSale = rand() % CLuaPlatform::defaultSale;
        for( int i=0; i < fMonth; i++ )
        {
            NrpTime ft( (*_object)[ STARTDATE ] );
            ft = ft.AppendMonth( i );

            //в моей игре цена приставки не может опуститься ниже 100$
            price = CNrpGameMarket::CalcAmpChange( price, 4, 100 );

            //ограничиваем скачки по продажам до 20%
            lastSale = CNrpGameMarket::CalcAmpChange( lastSale, 20, 100 );

            CNrpHistoryStep* step = hs->AddStep( ft );
            step->AddValue( COPYSELL, lastSale );
            step->AddValue( BALANCE, lastSale * price );
            step->AddValue( PRICE, price );
        }

         (*_object)[ PRICE ] = price;
    }
    
    return 0;
}

int CLuaPlatform::CreateRandomSales( lua_State* L )
{
    int argc = lua_gettop(L);
    luaL_argcheck(L, argc == 1, 1, "Function CLuaPlatform:CreateRandomSales not need parameter" );

    IF_OBJECT_NOT_NULL_THEN 
    {
        int price = (*_object)[ PRICE ];
        CNrpHistory* hs = (*_object)[ HISTORY ].As<CNrpHistory*>();
        assert( hs && price >= 100 );

        if( hs )
        {
            //в моей игре цена приставки не может опуститься ниже 100$
            price = CNrpGameMarket::CalcAmpChange( price, 4, 100.f );
            
            //получим продажи за последний месяц
            int lastSale = 0;
            CNrpHistoryStep* step = hs->GetLast();
            //если продаж не было
            lastSale = step ? (int)(*step)[ COPYSELL ] : ( rand() % CLuaPlatform::defaultSale );

            //ограничиваем скачки по продажам до 20%
            lastSale = CNrpGameMarket::CalcAmpChange( lastSale, 20, 100.f );

            step = hs->AddStep( _nrpApp[ CURRENTTIME ].As<NrpTime>() );
            step->AddValue( COPYSELL, lastSale );
            step->AddValue( PRICE, price );
            step->AddValue( BALANCE, lastSale * price );
        }
    }

    return 0;
}

const char* CLuaPlatform::ClassName()
{
	return CLASS_LUAPLATFORM;
}

int CLuaPlatform::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaPlatform:CreateTechnology need platformName parameter" );

	NrpText pathTo = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->Load( pathTo );
	return 0;
}

int CLuaPlatform::GetCpu( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, PROP, CPU, 0.f) );
	return 1;
}

int CLuaPlatform::GetRam( lua_State* L )
{
	lua_pushnumber( L, GetParam_<float>( L, PROP, RAM, 0.f) );
	return 1;
}

int CLuaPlatform::IsMyTech( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameProject:RemovePlatform need CLuaPlatform parameter" );

	CNrpTechnology* obj = _GetLuaObject< CNrpTechnology, CLuaTechnology >( L, 2, false );
	assert( obj );

	bool ret = false;
	IF_OBJECT_NOT_NULL_THEN 
		if( obj ) ret = ( _object->GetTech( (*obj)[ INTERNAL_NAME ] ) != NULL );

	lua_pushboolean( L, ret );
	return 1; 
}

}//end namespace nrp