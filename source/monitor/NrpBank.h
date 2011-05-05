#pragma once
#include "nrpConfig.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME LOANNUMBER( L"loanNumber" );
class CNrpLoan;
	
class CNrpBank : public INrpConfig
{
public:
	static CNrpBank& Instance();

	CNrpLoan* GetLoan( u32 index ) { return _loans[ index ]; }
	int GetLoanMoney( const NrpText& name );
	size_t GetMaxCompanyLoan( const NrpText& companyName );
	CNrpLoan* FindLoadByID( int id );
	void CreateLoan( const NrpText& name, int money, int percent, int month );

	static NrpText ClassName();

	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& saveFolder );

private:
	CNrpBank(void);
	~CNrpBank(void);

	NrpText _GetLoanSaveFileName( int index );

	LOANS _loans;
	int loanId_;
};

typedef CNrpBank* PNrpBank;

}//namespace nrp