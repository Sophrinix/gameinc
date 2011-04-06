#ifndef _INCLUDE_NRPBOOKKEEPING_H_
#define _INCLUDE_NRPBOOKKEEPING_H_

#include "NrpText.h"
#include "timeHelpers.h"

namespace nrp
{

class CNrpBookKeeping
{

public:
	void AddBill( const NrpText& name, int balance, const NrpTime& time );

	CNrpBookKeeping(void);
	~CNrpBookKeeping(void);
};

}//end namespace nrp
#endif
