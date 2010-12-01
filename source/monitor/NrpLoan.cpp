#include "StdAfx.h"
#include "NrpLoan.h"
#include "NrpCompany.h"

namespace nrp
{
CLASS_NAME CLASS_LOAN( "CNrpLoan" );

	
CNrpLoan::CNrpLoan( int id ) : INrpConfig( CLASS_LOAN, "" )
{
	Add<NrpText>( COMPANYNAME, "" );
	Add<float>( YEARPERCENT, 0 );
	Add<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	Add<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	Add<int>( MONEY, 0 );
	Add<int>( STARTMONEY, 0 );
	Add<int>( MONEYPAY, 0 );
	Add<int>( CLOSESHTRAF, 0 );
	Add<TYPE_LOAN>( TECHTYPE, TL_FREEMONEY );
	Add<int>( ID, id );
	Add<int>( MONTHLEFT, 0 );
}

CNrpLoan::~CNrpLoan(void)
{
}

NrpText CNrpLoan::ClassName()
{
	return CLASS_LOAN;
}

}//namespace nrp