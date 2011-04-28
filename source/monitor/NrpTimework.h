#ifndef _INCLUDE_NRPWEEKEND_H_
#define _INCLUDE_NRPWEEKEND_H_

#include "IWorkingModule.h"

namespace nrp
{
	
class NrpTimework : public IWorkingModule
{
public:
	NrpTimework( CNrpUser& user, const NrpTime& startDate, const NrpTime& endTime );

	virtual ~NrpTimework(void);

	int AddUser( CNrpUser& ptrUser );
	int RemoveUser( const NrpText& userName );
	const USERS& GetUsers() const { return _users; }
	
	void Update( CNrpUser& ptrUser, const NrpTime& time );

	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& fileName );

	static NrpText ClassName();

private:
	void _InitializeOptions();
	NrpTimework();

	USERS _users;
	CNrpUser* _user;
};

}//end namespace nrp

#endif
