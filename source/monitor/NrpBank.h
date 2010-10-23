#pragma once
#include "nrpConfig.h"
#include <vector>

namespace nrp
{
CLASS_NAME CLASS_BANK( "CNrpBank" );

OPTION_NAME LOANNUMBER( "loanNumber" );
class CNrpLoan;
	
class CNrpBank : public INrpConfig
{
	typedef std::vector< CNrpLoan* > LOAN_LIST;
public:
	static CNrpBank& Instance();

	CNrpLoan* GetLoan( size_t index ) { return loans_.at( index ); }
	size_t GetMaxCompanyLoan( std::string companyName );
	CNrpLoan* FindLoadByID( size_t id );
	void CreateLoan( std::string name, int money, int percent, int month );

	static std::string ClassName() { return CLASS_BANK; }
private:
	CNrpBank(void);
	~CNrpBank(void);

	LOAN_LIST loans_;
	int loanId_;
};

typedef CNrpBank* PNrpBank;

}//namespace nrp