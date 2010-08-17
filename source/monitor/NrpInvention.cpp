#include "StdAfx.h"
#include "NrpInvention.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "IUser.h"

#include <OleAuto.h>


namespace nrp
{

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

CNrpInvention::CNrpInvention( CNrpTechnology* pTech, CNrpCompany* pCmp ) : CNrpProjectModule( pTech, NULL )
{
	SetValue<PNrpCompany>( PARENTCOMPANY, pCmp );
	SetValue<TECH_STATUS>( STATUS, TS_INDEVELOP );
	CreateValue<int>( REALPRICE, _GetRealPrice( pTech ) );
	CreateValue<int>( PASSEDPRICE, 0 );
	CreateValue<int>( INVESTIMENT, 1000 );
	CreateValue<int>( DAYLEFT, 0 );
	CreateValue<int>( INVENTIONSPEED, 0 );
	CreateValue<int>( USERNUMBER, 0 );
	
	SYSTEMTIME time;
	memset( &time, 0, sizeof(SYSTEMTIME) );
	CreateValue<SYSTEMTIME>( PROGNOSEDATEFINISH, time );
	CheckParams();
}

void CNrpInvention::CheckParams()
{
	int needMoney = GetValue<int>( REALPRICE )-GetValue<int>( PASSEDPRICE);
	int dayToFinish = ( needMoney / GetValue<int>( INVESTIMENT )) * 30;

	double time;
	int errCurrTime = SystemTimeToVariantTime( &CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME ), &time );
	assert( errCurrTime > 0 );
	time += dayToFinish;

	VariantTimeToSystemTime( time, &GetValue<SYSTEMTIME>( PROGNOSEDATEFINISH ) );//прогноз завершения работ
	SetValue<int>( DAYLEFT, dayToFinish );//сколько дней осталось до завершения работ, при полном освоении финансирования
	
	int speed = ( GetValue<int>( INVESTIMENT ) * 3 ) / GetValue<int>( REALPRICE ) * 100;
	SetValue<int>( INVENTIONSPEED, speed );
}

CNrpInvention::~CNrpInvention(void)
{
}

void CNrpInvention::Update( IUser* ptrUser )
{
	PNrpCompany company = GetValue<PNrpCompany>( PARENTCOMPANY );
	assert( company != NULL );

	if( GetValue<int>( PASSEDPRICE ) < GetValue<int>( REALPRICE ) )
	{
		int reqSkill = 0;
		REQUIRE_MAP::iterator sIter = skillRequires_.begin();
		for( ; sIter != skillRequires_.end(); sIter++ )
			reqSkill += ptrUser->GetSkill( sIter->first );

		float genreSkill = ptrUser->GetGenreExperience( GetValue<PROJECT_TYPE>( TECHTYPE ) ) / 100.f;

		if( genreSkill < 0.1f )
			genreSkill = 0.1f;
		float genrePref = ptrUser->GetGenrePreferences( GetValue<PROJECT_TYPE>( TECHTYPE ) ) / 100.f;
		if( genrePref < 0.1f )
			genrePref = 0.1f;

		//человек может исследовать технологию в день на сумму не больше своей месячной зарплаты
		int money = static_cast<int>( reqSkill * (genrePref + genreSkill) * GetValue<int>( INVESTIMENT )) ;
		//if( money > ptrUser->GetValue<int>( SALARY ) )
		//	money = ptrUser->GetValue<int>( SALARY );

		int passedPrice = GetValue<int>( PASSEDPRICE ) + money;
		if( passedPrice >= GetValue<int>( REALPRICE ) )
			passedPrice = GetValue<int>( REALPRICE);

		SetValue<int>( PASSEDPRICE, passedPrice );
		SetValue<float>( READYWORKPERCENT, passedPrice / static_cast< float >( GetValue<int>( REALPRICE ) ) );
		int quality = GetValue<int>( QUALITY );
		SetValue<int>( QUALITY, (quality + ptrUser->GetValueA<int>( CODE_QUALITY )) / 2 );
	}

	if( GetValue<float>( READYWORKPERCENT ) >= 1 )
		CNrpApplication::Instance().InventionFinished( this );
}

IUser* CNrpInvention::GetUser( size_t index )
{
	assert( index < _users.size() );
	return index < _users.size() ? _users[ index ] : NULL;
}

int CNrpInvention::AddUser( IUser* user )
{
	USERS_LIST::iterator pIter = _users.begin();
	for( ; pIter != _users.end(); pIter++ )
		if( *pIter == user )
			return 1;
	
	_users.push_back( user );
	return 0;
}

int CNrpInvention::RemoveUser( std::string userName )
{
	USERS_LIST::iterator pIter = _users.begin();
	for( ; pIter != _users.end(); pIter++ )
		if( (*pIter)->GetValueA<std::string>( NAME ) == userName )
		{
			_users.erase( pIter );
			return 0;
		}

	return 1;
}
}//end namespace nrp