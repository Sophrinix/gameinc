#include "StdAfx.h"
#include "NrpLoan.h"
#include "NrpCompany.h"

namespace nrp
{
	
CNrpLoan::CNrpLoan( int id ) : INrpConfig( "CNrpLoan", "" )
{
	options_[ COMPANY ] = new PNrpCompany( NULL );
	options_[ YEARPERCENT ] = new float( 0 );
	options_[ STARTDATE ] = new SYSTEMTIME();
	options_[ ENDDATE ] = new SYSTEMTIME();
	options_[ MONEY ] = new int( 0 );
	options_[ STARTMONEY ] = new int( 0 );
	options_[ MONEYPAY ] = new int( 0 );
	options_[ CLOSESHTRAF ] = new int( 0 );
	options_[ TYPE ] = new TYPE_LOAN( TL_FREEMONEY );
	options_[ ID ] = new int( id );
	options_[ MONTHLEFT ] = new int( 0 );
}

CNrpLoan::~CNrpLoan(void)
{
}

}//namespace nrp