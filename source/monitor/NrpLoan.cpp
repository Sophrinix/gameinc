#include "StdAfx.h"
#include "NrpLoan.h"
#include "NrpCompany.h"

namespace nrp
{
CLASS_NAME CLASS_LOAN( "CNrpLoan" );

	
CNrpLoan::CNrpLoan( int id ) : INrpConfig( CLASS_LOAN, "" )
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
	Add( ID, id );
	Add( MONTHLEFT, 0 );
}

CNrpLoan::~CNrpLoan(void)
{
}

NrpText CNrpLoan::ClassName()
{
	return CLASS_LOAN;
}

}//namespace nrp