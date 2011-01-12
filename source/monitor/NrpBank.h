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
	int GetLoan( const NrpText& name );
	size_t GetMaxCompanyLoan( const NrpText& companyName );
	CNrpLoan* FindLoadByID( u32 id );
	void CreateLoan( const NrpText& name, int money, int percent, int month );

	static NrpText ClassName();
private:
	CNrpBank(void);
	~CNrpBank(void);

	LOANS _loans;
	int loanId_;
};

typedef CNrpBank* PNrpBank;

}//namespace nrp