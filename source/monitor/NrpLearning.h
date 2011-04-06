#ifndef _INCLUDE_NRPLEARNING_H_
#define _INCLUDE_NRPLEARNING_H_

#include "IWorkingModule.h"

namespace nrp
{

OPTION_NAME WORKNAME("workName" );
	
class NrpLearning : public IWorkingModule
{
public:
	NrpLearning( CNrpUser* user, const NrpTime& startDate, const NrpTime& endTime );

	virtual ~NrpLearning(void);

	int AddUser( CNrpUser* ptrUser );
	int RemoveUser( const NrpText& userName );

	void Update( CNrpUser* ptrUser, const NrpTime& time );

	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& fileName );

	static NrpText ClassName();

private:
	void _InitializeOptions();
	NrpLearning();
};

}//end namespace nrp

#endif
