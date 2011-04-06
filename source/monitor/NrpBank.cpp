#include "StdAfx.h"
#include "NrpBank.h"
#include "NrpApplication.h"
#include "NrpLoan.h"
#include "NrpCompany.h"

static nrp::CNrpBank* globalBankPointer=NULL;

namespace nrp
{
CLASS_NAME CLASS_BANK( L"CNrpBank" );

CNrpBank::CNrpBank(void) : INrpConfig( CLASS_BANK, "systemBank" )
{
	loanId_ = 0;
	Add<int>( LOANNUMBER, 0 );
}

CNrpBank::~CNrpBank(void)
{
}

size_t CNrpBank::GetMaxCompanyLoan( const NrpText& companyName )
{
	size_t dolg = 0;
	size_t result = 0;

	for( u32 i=0; i < _loans.size(); i++ )
	{
		if( (*_loans[ i ])[ COMPANYNAME ] == companyName )
			dolg += (int)(*_loans[ i ])[ MONEY ]; 
	}

	CNrpCompany* cmp = _nrpApp.GetCompany( companyName );

	if( cmp )
		result = (*cmp)[ BALANCE ] - static_cast< int >( dolg * 1.5f );

	return result > 10000 ? result * 2 : 0;
}

CNrpBank& CNrpBank::Instance()
{
	if( !globalBankPointer )
		globalBankPointer = new CNrpBank();

	return *globalBankPointer;
}

CNrpLoan* CNrpBank::FindLoadByID( int id )
{
	for( u32 i=0; i < _loans.size(); i++ )
		if( (*_loans[ i ])[ ID ] == id )
			return _loans[ i ];
	
	return NULL;
}

void CNrpBank::CreateLoan( const NrpText& name, int money, int percent, int month )
{
	CNrpLoan* loan = new CNrpLoan( loanId_++ );
	CNrpLoan& refLoan = *loan;
	NrpTime endtime( 0. ), time = _nrpApp[ CURRENTTIME ].As<NrpTime>();
	endtime = time;
	endtime.AppendMonth( month );

	refLoan[ YEARPERCENT ] = static_cast< float >( percent );
	refLoan[ STARTMONEY ] = money;
	refLoan[ MONEY ] = money;
	refLoan[ STARTDATE ] = time;
	refLoan[ MONTHLEFT ] = month;
	refLoan[ ENDDATE ] = endtime;
	PNrpCompany cmp = _nrpApp.GetCompany( name );
	assert( cmp );
	if( cmp )
		refLoan[ COMPANYNAME ] = (*cmp)[ NAME ];

	_loans.push_back( loan );
	(*cmp)[ BALANCE ] += money;

	_self[ LOANNUMBER ] = static_cast< int >( _loans.size() ); 
}

NrpText CNrpBank::ClassName()
{
	return CLASS_BANK;
}

int CNrpBank::GetLoanMoney( const NrpText& name )
{
	return 0;
}

}//namespace nrp