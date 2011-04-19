#include "StdAfx.h"
#include "NrpLearning.h"
#include "IUser.h"
#include "NrpConfigModificator.h"

namespace nrp
{

CLASS_NAME CLASS_NRPLEARNING( "NrpLearning" );

NrpLearning::~NrpLearning(void)
{
	CNrpUser& user = *(_self[ COMPONENTLIDER ].As<CNrpUser*>());

	int offset = _self[ OFFSET ];
	offset = ( (float)_self[ READYWORKPERCENT ] < 0.2 ) ? -offset : offset * (float)_self[ READYWORKPERCENT ];

	NrpText paramName = _self[ PARAMNAME ];
	if( user.IsExist( paramName ))
		user[ paramName ] += offset; 
}

NrpText NrpLearning::ClassName()
{
	return CLASS_NRPLEARNING;
}

NrpLearning::NrpLearning( CNrpUser& user, const NrpTime& startDate, const NrpTime& endDate ): IWorkingModule( PROJECT_TYPE(0), CLASS_NRPLEARNING )
{
	_InitializeOptions();
	_self[ TECHGROUP ] = (int)0;
	_self[ ENDDATE ] = endDate;
	_self[ STARTDATE ] = startDate;
	_self[ COMPONENTLIDER ] = &user;
}

void NrpLearning::_InitializeOptions()
{
	Add<CNrpUser*>( LASTWORKER, NULL );
	Add<int>( USERNUMBER, 0 );
	Add<int>( MONEYONDEVELOP, 0 );
	Add<NrpTime>( STARTDATE, NrpTime( 0. ) );
	Add<NrpText>( PARAMNAME, NrpText("") );
	Add<NrpTime>( LASTTIMEUPDATE, NrpTime( 0. ) );
	Add<int>( DURATION, 0 );
	Add<int>( OFFSET, 0 );
	Remove( PARENT );
	Add<INrpDevelopProject*>( PARENT, NULL );
}

void NrpLearning::Update( CNrpUser& ptrUser, const NrpTime& time )
{
	_self[ LASTTIMEUPDATE ] = time;

	NrpTime& startDate = _self[ STARTDATE ].As<NrpTime>();
	NrpTime& endDate = _self[ ENDDATE ].As<NrpTime>();
	int maxDay = startDate.GetDaysToDate( endDate );
	int raznica = const_cast< NrpTime& >( time ).GetDaysToDate( endDate );

	_self[ READYWORKPERCENT ] = static_cast< float >( 1.f - raznica / (float)maxDay ); 
}

NrpText NrpLearning::Save( const NrpText& saveFolder )
{
	return saveFolder;
}

void NrpLearning::Load( const NrpText& fileName )
{

}

int NrpLearning::AddUser( CNrpUser& ptrUser )
{
	return 0;
}

int NrpLearning::RemoveUser( const NrpText& userName )
{
	return 0;
}

}