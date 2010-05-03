#include "StdAfx.h"
#include "NrpLoan.h"
#include "NrpCompany.h"

namespace nrp
{
	
CNrpLoan::CNrpLoan( int id ) : INrpConfig( "CNrpLoan", "" )
{
	CreateValue<std::string>( COMPANYNAME, "" );
	CreateValue<float>( YEARPERCENT, 0 );
	CreateValue<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	CreateValue<SYSTEMTIME>( ENDDATE, SYSTEMTIME() );
	CreateValue<int>( MONEY, 0 );
	CreateValue<int>( STARTMONEY, 0 );
	CreateValue<int>( MONEYPAY, 0 );
	CreateValue<int>( CLOSESHTRAF, 0 );
	CreateValue<TYPE_LOAN>( TECHTYPE, TL_FREEMONEY );
	CreateValue<int>( ID, id );
	CreateValue<int>( MONTHLEFT, 0 );
}

CNrpLoan::~CNrpLoan(void)
{
}

}//namespace nrp