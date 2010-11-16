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

int CNrpInvention::_GetRealPrice( CNrpTechnology* tech )
{
	double stime, curtime;
	int errSTime = SystemTimeToVariantTime( &tech->GetValue<SYSTEMTIME>( STARTDATE ), &stime );
	int errCurrTime = SystemTimeToVariantTime( &CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME ), &curtime );
	assert( errSTime > 0 && errCurrTime > 0 );
	
	int month = static_cast<int>( stime - curtime ) / 30; //получаем количество месяцев от реального срока появления технологии на рынке
	float price = static_cast< float >( tech->GetValue<int>( BALANCE ) );
	for( int k=0; k < month; k++ )//каждый лишний месяц удорожает технологию на 10% от базовой стоимости( сложный процент )
		 price *= 1.1f;

	return static_cast<int>( price );
}

CNrpInvention::CNrpInvention( CNrpTechnology* pTech, CNrpCompany* pCmp ) 
			  : IWorkingModule( pTech->GetValue<PROJECT_TYPE>( TECHGROUP ), CLASS_INVENTION )
{
	InitializeOptions_();

	if( pTech && pCmp )
	{
		SetValue<int>( REALPRICE, _GetRealPrice( pTech ) );
		SetValue<PNrpCompany>( PARENTCOMPANY, pCmp );
		SetValue<NrpText>( COMPANYNAME, pCmp->GetValue<NrpText>( NAME ) );
		SetValue<TECH_STATUS>( STATUS, TS_INDEVELOP );
		SetValue<NrpText>( NAME, pTech->GetValue<NrpText>( NAME ) );
		SetValue<NrpText>( INTERNAL_NAME, pTech->GetValue<NrpText>( INTERNAL_NAME ) );
		SetValue<int>( TECHGROUP, pTech->GetValue<int>( TECHGROUP ) );
		SetValue<PROJECT_TYPE>( TECHTYPE, pTech->GetValue<PROJECT_TYPE>( TECHTYPE ) );
		SetValue<float>( BASE_CODE, pTech->GetValue<float>( BASE_CODE ) );
		SetValue<float>( ENGINE_CODE, pTech->GetValue<float>( ENGINE_CODE ) );
		SetValue<NrpText>( TEXTURENORMAL, pTech->GetValue<NrpText>( TEXTURENORMAL ) );
		SetValue<int>( LEVEL, pTech->GetValue<int>( LEVEL ) );
		SetValue<int>( QUALITY, pTech->GetValue<int>( QUALITY ) );
		SetValue<NrpText>( BASEFILE, pTech->GetValue<NrpText>( BASEFILE ) );
		SetValue<SYSTEMTIME>( USERSTARTDATE, CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME ) );

		SYSTEMTIME time;
		memset( &time, 0, sizeof(SYSTEMTIME) );
		CreateValue<SYSTEMTIME>( PROGNOSEDATEFINISH, time );
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
	CreateValue<int>( REALPRICE, 0 );
	CreateValue<int>( PASSEDPRICE, 0 );
	CreateValue<int>( INVESTIMENT, 1000 );
	CreateValue<int>( DAYLEFT, 0 );
	CreateValue<int>( INVENTIONSPEED, 0 );
	CreateValue<int>( USERNUMBER, 0 );
	CreateValue<NrpText>( COMPANYNAME, "" );
	CreateValue<SYSTEMTIME>( USERSTARTDATE, SYSTEMTIME() );
	CreateValue<int>( MONEY_TODECREASE, 0 );
}

void CNrpInvention::CheckParams()
{
	int dayFromStart = TimeHelper::GetDaysBetweenDate( GetValue<SYSTEMTIME>( USERSTARTDATE ),
													   CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME) ); 

	//получим скока тратим в день на исследования
	int moneyInDay = GetValue<int>( PASSEDPRICE ) / dayFromStart;
	//вычислим сколько дней осталось до конца при нынешних затратах
	assert( moneyInDay > 0 );
	if( moneyInDay == 0 )
		moneyInDay = 1;
	int dayToFinish = ( GetValue<int>( REALPRICE ) - GetValue<int>( PASSEDPRICE) ) / moneyInDay;
	
	SetValue<SYSTEMTIME>( PROGNOSEDATEFINISH, TimeHelper::GetDateWithDay( CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME),
																		  dayToFinish ) );

	SetValue<int>( DAYLEFT, dayToFinish );//сколько дней осталось до завершения работ, при полном освоении финансирования
	SetValue<int>( INVENTIONSPEED, moneyInDay );
}

CNrpInvention::~CNrpInvention(void)
{
}

void CNrpInvention::Update( IUser* ptrUser )
{
	PNrpCompany company = GetValue<PNrpCompany>( PARENTCOMPANY );
	assert( ptrUser != NULL && company != NULL );

	if( GetValue<int>( PASSEDPRICE ) < GetValue<int>( REALPRICE ) )
	{
		int reqSkill = 0;
		REQUIRE_MAP::Iterator sIter = _skillRequires.getIterator();
		
		for( ; !sIter.atEnd(); sIter++ )
			reqSkill += ptrUser->GetSkill( sIter->getKey() );

		float genreSkill = ptrUser->GetGenreExperience( GetValue<PROJECT_TYPE>( TECHTYPE ) ) / 100.f;

		if( genreSkill < 0.1f )
			genreSkill = 0.1f;
		float genrePref = ptrUser->GetGenrePreferences( GetValue<PROJECT_TYPE>( TECHTYPE ) ) / 100.f;
		if( genrePref < 0.1f )
			genrePref = 0.1f;

		float updateMoney = reqSkill * (genrePref + genreSkill) / ( 30.f * 8 );
		float maxUpdateMoney = ptrUser->GetValue<int>( SALARY ) / 8.f;
		float money = updateMoney * GetValue<int>( INVESTIMENT );
		//человек может исследовать технологию в день на сумму не больше своей месячной зарплаты
		money = min( money, maxUpdateMoney );

		//эти средства реально пошли на изучение новых технологий
		AddValue<int>( PASSEDPRICE, static_cast<int>( money ) );
		//эту сумму надо списать у конторы в конце месяца
		AddValue<int>( MONEY_TODECREASE, GetValue<int>( INVESTIMENT ) / ( 30 * 8 ) );
	
		SetValue<float>( READYWORKPERCENT, GetValue<int>(PASSEDPRICE) / static_cast< float >( GetValue<int>( REALPRICE ) ) );
		AddValue<int>( QUALITY, (GetValue<int>( QUALITY ) + ptrUser->GetValue<int>( CODE_QUALITY )) / 2 );
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
	SetValue<int>( USERNUMBER, _users.size() );
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
	return GetString( INTERNAL_NAME ) + ".invent";
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
	return ( GetString( NAME ) == name || GetString( INTERNAL_NAME ) == name ) && ( GetString( COMPANYNAME ) == company );
}

NrpText CNrpInvention::ClassName()
{
	return CLASS_INVENTION;
}

}//end namespace nrp