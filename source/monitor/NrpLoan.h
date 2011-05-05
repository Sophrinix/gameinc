#pragma once
#include "nrpConfig.h"

namespace nrp
{
OPTION_NAME YEARPERCENT( "percentInYear");
OPTION_NAME MONEY( "money" );
OPTION_NAME STARTMONEY( "startMoney" );
OPTION_NAME MONEYPAY( "moneyPay" );
OPTION_NAME CLOSESHTRAF( "closeShtraf" );
OPTION_NAME ID( "id" );
OPTION_NAME MONTHLEFT( "monthLeft" );
OPTION_NAME LOANTYPE( "loanType" );

class CNrpLoan : public INrpConfig
{
public:
	typedef enum { TL_FREEMONEY=0, TL_TIMEMONEY, TL_COUNT } TYPE_LOAN;
	CNrpLoan( int id );
	CNrpLoan( const NrpText& fileName );
	~CNrpLoan(void);

	static NrpText ClassName();

	NrpText Save( const NrpText& saveFile );
	void Load( const NrpText& saveFile );

private:
	void _InitOptions();
};

}//namespace nrp