#include "StdAfx.h"

#include <irrlicht.h>

#include "LuaChart.h"
#include "nrpChartCtrl.h"
#include "nrpChartAxis.h"
#include "nrpChartSerie.h"
#include "StrConversation.h"
using namespace irr;

namespace nrp
{

Luna< CLuaChart >::RegType CLuaChart::methods[] =			//���������� ������
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaChart ),
	/*    */
	LUNA_AUTONAME_FUNCTION( CLuaChart, SetAxisAutomatic ),
	LUNA_AUTONAME_FUNCTION( CLuaChart, Update ),
	LUNA_AUTONAME_FUNCTION( CLuaChart, AddSerie ),
	LUNA_AUTONAME_FUNCTION( CLuaChart, AddPoint ),
	LUNA_AUTONAME_FUNCTION( CLuaChart, UpdateData ),
	LUNA_AUTONAME_FUNCTION( CLuaChart, SetOffsetPoints ),
	{0,0}
};

CLuaChart::CLuaChart(lua_State *L)	: ILuaGuiElement(L, "CLuaChart")							//�����������
{}

int CLuaChart::SetAxisAutomatic( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaElement::SetAxisAutomatic need 2 parameter");

	int aType = lua_tointeger( L, 2 );
	bool automatic = lua_toboolean( L, 3 ) > 0;
	IF_OBJECT_NOT_NULL_THEN object_->GetAxis( gui::CChartCtrl::CHART_AXIS_NAME(aType) )->SetAutomatic( automatic );

	return 1;
}

int CLuaChart::Update( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaElement::Update not need any parameter");

	IF_OBJECT_NOT_NULL_THEN object_->Update();

	return 1;
}

int CLuaChart::AddSerie( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaElement::AddSerie need 1 parameter");

	int sType = lua_tointeger( L, 2 );
	gui::CChartSerie* ptrSerie = NULL;
	IF_OBJECT_NOT_NULL_THEN ptrSerie = object_->AddSerie( gui::CChartSerie::SERIES_TYPE( sType ) );

	lua_pushinteger( L, ptrSerie == NULL ? -1 : (object_->GetSeriesCount() - 1) );

	return 1;
}

int CLuaChart::AddPoint( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaElement::AddSerie need 3 parameter");

    int numSerie = lua_tointeger( L, 2 );
	float x = (float)lua_tonumber( L, 3 );
	float y = (float)lua_tonumber( L, 4 );
	
	IF_OBJECT_NOT_NULL_THEN
	{
		gui::CChartSerie* ptrSerie = object_->GetSerie( numSerie );
		if( ptrSerie )
			ptrSerie->AddPoint( x, y );
	}

	return 1;
}

int CLuaChart::UpdateData( lua_State *L )			//���������� ��������
{
	/*int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaElement::UpdateData need 4 parameter");

	size_t serieNumber = (size_t)lua_tointeger( L, 2 );
	void* buffer = lua_touserdata( L, 3 );
	int lenght = lua_tointeger( L, 4 );
	std::string dataType = lua_tostring( L, 5 );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( serieNumber >= object_->GetSeriesCount() )
			return 1;

		gui::CChartSerie* serie = object_->GetSerie( serieNumber );
		serie->Clear();
		if( dataType == CHNL_RFYZ_ASRF )
		{
			DWORD* rfyz = (DWORD*)buffer;

			for( int cnt=0; cnt < lenght; cnt++ )
				serie->AddPoint( cnt, rfyz[ cnt ] & 0x000000ff );
		}

		object_->Update();
	}

*/
	return 1;
}

int CLuaChart::SetOffsetPoints( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaElement::SetOffsetPoints need 3 parameter");

	size_t serieNumber = (size_t)lua_tointeger( L, 2 );
	float xOffset = (float)lua_tonumber( L, 3 );
	float yOffset = (float)lua_tonumber( L, 4 );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( serieNumber >= object_->GetSeriesCount() )
			return 1;

		object_->GetSerie( serieNumber )->SetOffset( xOffset, yOffset );

		object_->Update();
	}


	return 1;
}
}//namespace nrp