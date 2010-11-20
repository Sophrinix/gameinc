#include "StdAfx.h"
#include "NrpInvention.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "IUser.h"
#include "timeHelpers.h"
#include "OpFileSystem.h"
#include "IniFile.h"

#include <OleAuto.h>

namespace nrp
{
CLASS_NAME CLASS_INVENTION( "CNrpInvention" );

int CNrpInvention::_GetRealPrice( CNrpTechnology& tech )
{
	double stime, curtime;
	int errSTime = SystemTimeToVariantTime( &(tech[ STARTDATE ].As<SYSTEMTIME>()), &stime );
	int errCurrTime = SystemTimeToVariantTime( &(CNrpApplication::Instance()[ CURRENTTIME ].As<SYSTEMTIME>()), &curtime );
	assert( errSTime > 0 && errCurrTime > 0 );
	
	int month = static_cast<int>( stime - curtime ) / 30; //получаем количество месяцев от реального срока появления технологии на рынке
	int price = tech[ BALANCE ];
	for( int k=0; k < month; k++ )//каждый лишний месяц удорожает технологию на 10% от базовой стоимости( сложный процент )
		 price = static_cast< int >( price * 1.1f );

	return price;
}

CNrpInvention::CNrpInvention( CNrpTechnology* pTech, CNrpCompany* pCmp ) 
			  : IWorkingModule( pTech->Param( TECHGROUP ).As<PROJECT_TYPE>(), CLASS_INVENTION )
{
	InitializeOptions_();

	if( pTech && pCmp )
	{
		const CNrpTechnology& refTech = * pTech;
		Param( REALPRICE ) = _GetRealPrice( *pTech );
		Param( PARENTCOMPANY ) = pCmp;
		Param( COMPANYNAME ) = (*pCmp)[ NAME ];
		Param( STATUS ) = TS_INDEVELOP;
		Param( NAME ) = refTech[ NAME ];
		Param( INTERNAL_NAME ) = refTech[ INTERNAL_NAME ];
		Param( TECHGROUP ) = refTech[ TECHGROUP ];
		Param( TECHTYPE ) = refTech[ TECHTYPE ];
		Param( BASE_CODE ) = refTech[ BASE_CODE ];
		Param( ENGINE_CODE ) = refTech[ ENGINE_CODE ];
		Param( TEXTURENORMAL ) = refTech[ TEXTURENORMAL ];
		Param( LEVEL ) = refTech[ LEVEL ];
		Param( QUALITY ) = refTech[ QUALITY ];
		Param( BASEFILE ) = refTech[ BASEFILE ];
		Param( USERSTARTDATE ) = CNrpApplication::Instance()[ CURRENTTIME ];

		SYSTEMTIME time;
		memset( &time, 0, sizeof(SYSTEMTIME) );
		Push<SYSTEMTIME>( PROGNOSEDATEFINISH, time );
		CheckParams();

		CopyMapTo( _techRequires, pTech->GetTechRequires() );
		CopyMapTo( _skillRequires,  pTech->GetSkillRequires() );
	}
}

CNrpInvention::CNrpInvention( const NrpText& fileName ) 
			  : IWorkingModule( PROJECT_TYPE(0), CLASS_INVENTION )
{
	InitializeOptions_();
	Load( fileName );
}

void CNrpInvention::InitializeOptions_()
{
	Push<int>( REALPRICE, 0 );
	Push<int>( PASSEDPRICE, 0 );
	Push<int>( INVESTIMENT, 1000 );
	Push<int>( DAYLEFT, 0 );
	Push<int>( INVENTIONSPEED, 0 );
	Push<int>( USERNUMBER, 0 );
	Push<NrpText>( COMPANYNAME, "" );
	Push<SYSTEMTIME>( USERSTARTDATE, SYSTEMTIME() );
	Push<int>( MONEY_TODECREASE, 0 );
}

void CNrpInvention::CheckParams()
{
	int dayFromStart = TimeHelper::GetDaysBetweenDate( Param( USERSTARTDATE ),
													   CNrpApplication::Instance()[ CURRENTTIME ] ); 

	//получим скока тратим в день на исследования
	int moneyInDay = (int)Param( PASSEDPRICE ) / dayFromStart;
	//вычислим сколько дней осталось до конца при нынешних затратах
	assert( moneyInDay > 0 );
	if( moneyInDay == 0 )
		moneyInDay = 1;
	int dayToFinish = ( (int)Param( REALPRICE ) - (int)Param( PASSEDPRICE) ) / moneyInDay;
	
	Param( PROGNOSEDATEFINISH ) = TimeHelper::GetDateWithDay( CNrpApplication::Instance()[ CURRENTTIME ], dayToFinish );

	Param( DAYLEFT ) = dayToFinish;//сколько дней осталось до завершения работ, при полном освоении финансирования
	Param( INVENTIONSPEED ) = moneyInDay;
}

CNrpInvention::~CNrpInvention(void)
{
}

void CNrpInvention::Update( IUser* ptrUser )
{
	PNrpCompany company = Param( PARENTCOMPANY ).As<PNrpCompany>();
	assert( ptrUser != NULL && company != NULL );

	if( Param( PASSEDPRICE ) < Param( REALPRICE ) )
	{
		int reqSkill = 0;
		REQUIRE_MAP::Iterator sIter = _skillRequires.getIterator();
		
		for( ; !sIter.atEnd(); sIter++ )
			reqSkill += ptrUser->GetSkill( sIter->getKey() );

		float genreSkill = ptrUser->GetGenreExperience( Param( TECHTYPE ).As<GENRE_TYPE>() ) / 100.f;

		if( genreSkill < 0.1f )
			genreSkill = 0.1f;
		float genrePref = ptrUser->GetGenrePreferences( Param( TECHTYPE ).As<GENRE_TYPE>() ) / 100.f;
		if( genrePref < 0.1f )
			genrePref = 0.1f;

		float updateMoney = reqSkill * (genrePref + genreSkill) / ( 30.f * 8 );
		float maxUpdateMoney = (int)ptrUser->Param( SALARY ) / 8.f;
		float money = updateMoney * (int)Param( INVESTIMENT );
		//человек может исследовать технологию в день на сумму не больше своей месячной зарплаты
		money = min( money, maxUpdateMoney );

		//эти средства реально пошли на изучение новых технологий
		Param( PASSEDPRICE ) += static_cast<int>( money );
		//эту сумму надо списать у конторы в конце месяца
		Param( MONEY_TODECREASE ) += (int)Param( INVESTIMENT ) / ( 30 * 8 );
	
		Param( READYWORKPERCENT ) = (int)Param(PASSEDPRICE) / static_cast< float >( (int)Param( REALPRICE ) );
		Param( QUALITY ) = ( (int)Param( QUALITY ) + (int)ptrUser->Param( CODE_QUALITY ) ) / 2;
	}
}

IUser* CNrpInvention::GetUser( u32 index )
{
	assert( index < _users.size() );
	return index < _users.size() ? _users[ index ] : NULL;
}

int CNrpInvention::AddUser( IUser* user )
{
	for( u32 i=0; i < _users.size(); i++ )
		if( _users[ i ] == user )
			return 1;
	
	_users.push_back( user );
	Param( USERNUMBER ) = static_cast< int >(_users.size() );
	user->AddWork( this, true );
	return 0;
}

int CNrpInvention::RemoveUser( const NrpText& userName )
{
	for( u32 i=0; i < _users.size(); i++ )
		if( _users[ i ]->Equale( userName ) )
			_users.erase( i );

	return 1;
}

void CNrpInvention::Load( const NrpText& fileName )
{
	INrpProject::Load( fileName );
	IniFile rv( fileName );

	rv.Get( SECTION_REQUIRE_TECH, _techRequires );
	rv.Get( SECTION_REQUIRE_SKILL, _skillRequires );
}

NrpText CNrpInvention::_GetFileName()
{
	return (NrpText)Text( INTERNAL_NAME ) + ".invent";
}

NrpText CNrpInvention::Save( const NrpText& saveFolder, bool k )
{
	NrpText myFolder = OpFileSystem::CheckEndSlash( saveFolder );

	OpFileSystem::CreateDirectory( myFolder );

	NrpText fileName = myFolder + _GetFileName();

	assert( !OpFileSystem::IsExist( fileName ) );
	INrpProject::Save( fileName );
	IniFile sv( fileName );
	assert( OpFileSystem::IsExist( fileName ) ); 
	
	sv.Set( SECTION_REQUIRE_TECH, _techRequires );
	sv.Set( SECTION_REQUIRE_SKILL, _skillRequires );
	sv.SetArray< USERS >( SECTION_USERS, _users, CreateKeyUser, NAME, false );

	return fileName;
}

bool CNrpInvention::Equale( const NrpText& name, const NrpText& company )
{
	return ( Text( NAME ) == name || Text( INTERNAL_NAME ) == name ) && ( Text( COMPANYNAME ) == company );
}

NrpText CNrpInvention::ClassName()
{
	return CLASS_INVENTION;
}

}//end namespace nrp