#include "StdAfx.h"
#include "NrpLoan.h"
#include "NrpCompany.h"

namespace nrp
{
CLASS_NAME CLASS_LOAN( "CNrpLoan" );

void CNrpLoan::_InitOptions()
{
	Add<NrpText>( COMPANYNAME, "" );
	Add( YEARPERCENT, 0.f );
	Add( STARTDATE, NrpTime( 0. ) );
	Add( ENDDATE, NrpTime( 0. ) );
	Add( MONEY, 0 );
	Add( STARTMONEY, 0 );
	Add( MONEYPAY, 0 );
	Add( CLOSESHTRAF, 0 );
	Add( LOANTYPE, (int)TL_FREEMONEY );
	Add( ID, 0 );
	Add( MONTHLEFT, 0 );
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