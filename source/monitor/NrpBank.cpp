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
	Push<int>( LOANNUMBER, 0 );
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
		if( _loans[ i ]->GetString( COMPANYNAME ) == companyName )
			dolg += _loans[ i ]->GetValue<int>( MONEY ); 
	}

	CNrpCompany* cmp = CNrpApplication::Instance().GetCompany( companyName );

	if( cmp )
		result = (int)(cmp->GetValue<int>( BALANCE ) - dolg * 1.5);

	return result > 10000 ? result * 2 : 0;
}

CNrpBank& CNrpBank::Instance()
{
	if( !globalBankPointer )
		globalBankPointer = new CNrpBank();

	return *globalBankPointer;
}

CNrpLoan* CNrpBank::FindLoadByID( u32 id )
{
	for( u32 i=0; i < _loans.size(); i++ )
		if( _loans[ i ]->GetValue<int>( ID ) == id )
			return _loans[ i ];

	return NULL;
}

void CNrpBank::CreateLoan( const NrpText& name, int money, int percent, int month )
{
	CNrpLoan* loan = new CNrpLoan( loanId_++ );
	SYSTEMTIME endtime, time = CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME );
	endtime = time;
	endtime.wYear = time.wYear + month / 12;
	endtime.wMonth = ( time.wMonth + month ) % 12;
	loan->SetValue<int>( YEARPERCENT, percent );
	loan->SetValue<int>( STARTMONEY, money );
	loan->SetValue<int>( MONEY, money );
	loan->SetValue<SYSTEMTIME>( STARTDATE, time );
	loan->SetValue<int>( MONTHLEFT, month );
	loan->SetValue<SYSTEMTIME>( ENDDATE, endtime );
	PNrpCompany cmp = CNrpApplication::Instance().GetCompany( name );
	loan->SetString( COMPANYNAME, cmp->GetString( NAME ) );

	_loans.push_back( loan );
	cmp->AddValue<int>( BALANCE, money );

	int lNum = _loans.size();
	SetValue<int>( LOANNUMBER, lNum ); 
}

NrpText CNrpBank::ClassName()
{
	return CLASS_BANK;
}

}//namespace nrp