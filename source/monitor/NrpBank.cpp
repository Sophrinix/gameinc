#include "StdAfx.h"
#include "NrpBank.h"
#include "NrpApplication.h"
#include "NrpLoan.h"
#include "NrpCompany.h"

static nrp::CNrpBank* globalBankPointer=NULL;

namespace nrp
{

CNrpBank::CNrpBank(void) : INrpConfig( CLASS_BANK, "systemBank" )
{
	loanId_ = 0;
	CreateValue<int>( LOANNUMBER, 0 );
}

CNrpBank::~CNrpBank(void)
{
}

size_t CNrpBank::GetMaxCompanyLoan( std::string companyName )
{
	LOAN_LIST::iterator pIter = loans_.begin();

	size_t dolg = 0;
	size_t result = 0;

	for( ; pIter != loans_.end(); pIter++ )
	{
		if( (*pIter)->GetValue<std::string>( COMPANYNAME ) == companyName )
			dolg += (*pIter)->GetValue<int>( MONEY ); 
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

CNrpLoan* CNrpBank::FindLoadByID( size_t id )
{
	LOAN_LIST::iterator pIter = loans_.begin();

	for( ; pIter != loans_.end(); pIter++ )
		if( (*pIter)->GetValue<int>( ID ) == id )
			return (*pIter);

	return NULL;
}

void CNrpBank::CreateLoan( std::string name, int money, int percent, int month )
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
	loan->SetValue<std::string>( COMPANYNAME, cmp->GetValue<std::string>( NAME ) );

	loans_.push_back( loan );
	cmp->AddValue<int>( BALANCE, money );

	int lNum = loans_.size();
	SetValue<int>( LOANNUMBER, lNum ); 
}
}//namespace nrp