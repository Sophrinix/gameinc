#include "StdAfx.h"
#include "NrpTimework.h"
#include "IUser.h"
#include "NrpConfigModificator.h"

namespace nrp
{

CLASS_NAME CLASS_NRPWEEKEND( "NrpWeekend" );

NrpTimework::~NrpTimework(void)
{
	CNrpUser* user = _self[ COMPONENTLIDER ].As<CNrpUser*>();
	if( user )
	{
		int offset = _self[ OFFSET ];
		offset = (float)_self[ READYWORKPERCENT ] < 0.2 ? -offset : offset * (float)_self[ READYWORKPERCENT ];

		NrpTime time = _self[ LASTTIMEUPDATE ].As<NrpTime>().AppendDay( (int)_self[ DURATION ] );
		user->AddModificator( new CNrpConfigModificator<int>( user, time, _self[ PARAMNAME ], false, offset, _self[ NAME ] ) );
	}
}

NrpText NrpTimework::ClassName()
{
	return CLASS_NRPWEEKEND;
}

NrpTimework::NrpTimework( CNrpUser& user, const NrpTime& startDate, const NrpTime& endDate ): IWorkingModule( PROJECT_TYPE(0), CLASS_NRPWEEKEND )
{
	_InitializeOptions();
	_self[ TECHGROUP ] = (int)0;
	_self[ ENDDATE ] = endDate;
	_self[ STARTDATE ] = startDate;
	_self[ COMPONENTLIDER ] = &user;
}

void NrpTimework::_InitializeOptions()
{
	RegProperty<CNrpUser*>( LASTWORKER, NULL );
	RegProperty( USERNUMBER, 0 );
	RegProperty( MONEYONDEVELOP, 0 );
	RegProperty( STARTDATE, NrpTime( 0. ) );
	RegProperty( PARAMNAME, NrpText("") );
	RegProperty( LASTTIMEUPDATE, NrpTime( 0. ) );
	RegProperty( DURATION, 0 );
	RegProperty( OFFSET, 0 );
	UnregProperty( PARENT );
	RegProperty<INrpDevelopProject*>( PARENT, NULL );
}

void NrpTimework::Update( CNrpUser& ptrUser, const NrpTime& time )
{
	_self[ LASTTIMEUPDATE ] = time;

	NrpTime& startDate = _self[ STARTDATE ].As<NrpTime>();
	NrpTime& endDate = _self[ ENDDATE ].As<NrpTime>();
	int maxDay = startDate.GetDaysToDate( endDate );
	int raznica = const_cast< NrpTime& >( time ).GetDaysToDate( endDate );

	_self[ READYWORKPERCENT ] = static_cast< float >( 1.f - raznica / (float)maxDay ); 
}

NrpText NrpTimework::Save( const NrpText& saveFolder )
{

	return saveFolder;
}

void NrpTimework::Load( const NrpText& fileName )
{

}

int NrpTimework::AddUser( CNrpUser& ptrUser )
{
	return 0;
}

int NrpTimework::RemoveUser( const NrpText& userName )
{
	return 0;
}

}