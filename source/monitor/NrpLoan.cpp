#include "StdAfx.h"
#include "NrpLoan.h"
#include "NrpCompany.h"

namespace nrp
{
CLASS_NAME CLASS_LOAN( "CNrpLoan" );

void CNrpLoan::_InitOptions()
{
	RegProperty<NrpText>( COMPANYNAME, "" );
	RegProperty( YEARPERCENT, 0.f );
	RegProperty( STARTDATE, NrpTime( 0. ) );
	RegProperty( ENDDATE, NrpTime( 0. ) );
	RegProperty( MONEY, 0 );
	RegProperty( STARTMONEY, 0 );
	RegProperty( MONEYPAY, 0 );
	RegProperty( CLOSESHTRAF, 0 );
	RegProperty( LOANTYPE, (int)TL_FREEMONEY );
	RegProperty( ID, 0 );
	RegProperty( MONTHLEFT, 0 );
}
	
CNrpLoan::CNrpLoan( int id ) : INrpConfig( CLASS_LOAN, "" )
{
	_InitOptions();
	_self[ ID ] = id;
}

CNrpLoan::CNrpLoan( const NrpText& fileName ): INrpConfig( CLASS_LOAN, "" )
{
	_InitOptions();
	Load( fileName );
}

CNrpLoan::~CNrpLoan(void)
{
}

NrpText CNrpLoan::ClassName()
{
	return CLASS_LOAN;
}

NrpText CNrpLoan::Save( const NrpText& saveFile )
{
	return INrpConfig::Save( saveFile );
}

void CNrpLoan::Load( const NrpText& saveFile )
{
	INrpConfig::Load( saveFile );
}

}//namespace nrp