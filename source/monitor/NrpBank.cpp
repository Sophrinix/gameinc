#include "StdAfx.h"
#include "NrpBank.h"
#include "NrpApplication.h"
#include "NrpLoan.h"
#include "NrpCompany.h"

static nrp::CNrpBank* globalBankPointer=NULL;

namespace nrp
{

CNrpBank::CNrpBank(void) : INrpConfig( "CNrpBank", "systemBank" )
{
	loanId_ = 0;
	options_[ LOANNUMBER ] = new int( 0 );
}

CNrpBank::~CNrpBank(void)
{
}

size_t CNrpBank::GetMaxCompanyLoan( std::string companyName )
{
	LOAN_LIST::iterator pIter = loans_.begin();

	size_t dolg = 0;
	size_t result = 0;

	for( ; pIter != loans_.end(); ++pIter )
	{
		if( (*pIter)->GetOption<PNrpCompany>( COMPANY )->GetOption<std::string>( NAME ) == companyName )
			dolg += (*pIter)->GetOption<int>( MONEY ); 
	}

	CNrpCompany* cmp = CNrpApplication::Instance().GetCompany( companyName );

	if( cmp )
		result = (int)(cmp->GetOption<int>( BALANCE ) - dolg * 1.5);

	return result > 10000 ? result * 2 : 0;
}

CNrpBank& CNrpBank::Instance()
{
	if( !globalBankPointer )
		globalBankPointer = new CNrpBank();

	return *globalBankPointer;
}

CNrpLoan* CNrpBank::FindLoadByID( size_t id )
{
	LOAN_LIST::iterator pIter = loans_.begin();

	for( ; pIter != loans_.end(); ++pIter )
		if( (*pIter)->GetOption<int>( ID ) == id )
			return (*pIter);

	return NULL;
}

void CNrpBank::CreateLoan( std::string name, int money, int percent, int month )
{
	CNrpLoan* loan = new CNrpLoan( ++loanId_ );
	SYSTEMTIME endtime, time = CNrpApplication::Instance().GetDateTime();
	endtime = time;
	endtime.wYear = time.wYear + month / 12;
	endtime.wMonth = ( time.wMonth + month ) % 12;
	loan->SetOption<int>( YEARPERCENT, percent );
	loan->SetOption<int>( STARTMONEY, money );
	loan->SetOption<int>( MONEY, money );
	loan->SetOption<SYSTEMTIME>( STARTDATE, time );
	loan->SetOption<int>( MONTHLEFT, month );
	loan->SetOption<SYSTEMTIME>( ENDDATE, endtime );
	PNrpCompany cmp = CNrpApplication::Instance().GetCompany( name );
	loan->SetOption<PNrpCompany>( COMPANY, cmp );

	loans_.push_back( loan );

	int bnm = cmp->GetOption<int>( BALANCE ) + money;
	cmp->SetOption<int>( BALANCE, bnm );

	int lNum = loans_.size();
	SetOption<int>( LOANNUMBER, lNum ); 
}
}//namespace nrp