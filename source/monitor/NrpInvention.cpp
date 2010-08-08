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
		 price *= 0.1f;

	return static_cast<int>( price );
}

CNrpInvention::CNrpInvention( CNrpTechnology* pTech, CNrpCompany* pCmp ) : CNrpProjectModule( pTech, NULL )
{
	SetValue<PNrpCompany>( PARENTCOMPANY, pCmp );
	SetValue<TECH_STATUS>( STATUS, TS_INDEVELOP );
	CreateValue<int>( REALPRICE, _GetRealPrice( pTech ) );
	CreateValue<int>( PASSEDPRICE, 0 );
	CreateValue<int>( INVESTIMENT, 0 );
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
}//end namespace nrp