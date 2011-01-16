#include "StdAfx.h"
#include "NrpBridge.h"
#include "NrpCompany.h"
#include "NrpBank.h"
#include "NrpApplication.h"

using namespace irr;
static nrp::CNrpBridge * globalBridgeInstance = 0;

namespace nrp 
{
CLASS_NAME CLASS_NRPBRIDGE( L"CNrpBridge" );

CNrpBridge::CNrpBridge(void) : INrpConfig( CLASS_NRPBRIDGE, CLASS_NRPBRIDGE )
{
	Add<int>( COMPANIESNUMBER, 0 );
}

CNrpBridge& CNrpBridge::Instance()
{
	if( !globalBridgeInstance)
		globalBridgeInstance = new CNrpBridge();

	return *globalBridgeInstance;
}

NrpText CNrpBridge::ClassName()
{
	return CLASS_NRPBRIDGE;
}

void CNrpBridge::_CheckCompany( CNrpCompany* company )
{
	assert( company );

	CNrpCompany& refCpm = *company;

	//наличка + активы
	int selfCapital = refCpm[ SELF_CAPITAL ];
	int otherCapital = refCpm[ OTHER_CAPITAL ];
	int investMoney = selfCapital + otherCapital;

	//долги
	int loanMoney = CNrpBank::Instance().GetLoan( (NrpText)refCpm[ NAME ] );
	
	//предполагаемый доход на свой капитал
	float selfDividend = (float)refCpm[ MONEY_ON_PIE ] / (float)refCpm[ PIE_COST ];
	float koeff1 = ( selfCapital - loanMoney ) / investMoney * selfDividend;

	//предполагаемый доход на добавочный капитал
	float middleDividend = _GetMiddlePercent( (NrpText)refCpm[ NAME ] );
	float koeff2 = otherCapital / investMoney * middleDividend * ( 1 - (float)_nrpApp[ TAX ] );

	float piePrice = investMoney / (int)refCpm[ PIE_NUMBER ] * (koeff1 + koeff2);

	refCpm[ PIE_COST ] = piePrice;
}

void CNrpBridge::Update()
{
	for( u32 i=0; i < _companies.size(); i++ )
		 _CheckCompany( _companies[ i ] );
}

float CNrpBridge::_GetMiddlePercent( const NrpText& name )
{
	return 0;
}

}//namespace nrp