#include "StdAfx.h"
#include "NrpLoan.h"
#include "NrpCompany.h"

namespace nrp
{
CLASS_NAME CLASS_LOAN( "CNrpLoan" );

	
CNrpLoan::CNrpLoan( int id ) : INrpConfig( CLASS_LOAN, "" )
{
	Push<NrpText>( COMPANYNAME, "" );
	Push<float>( YEARPERCENT, 0 );
	Push<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	Push<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	Push<int>( MONEY, 0 );
	Push<int>( STARTMONEY, 0 );
	Push<int>( MONEYPAY, 0 );
	Push<int>( CLOSESHTRAF, 0 );
	Push<TYPE_LOAN>( TECHTYPE, TL_FREEMONEY );
	Push<int>( ID, id );
	Push<int>( MONTHLEFT, 0 );
}

CNrpLoan::~CNrpLoan(void)
{
}

NrpText CNrpLoan::ClassName()
{
	return CLASS_LOAN;
}

}//namespace nrp