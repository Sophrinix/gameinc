#pragma once
#include "INrpProject.h"

namespace nrp
{
class CNrpRetailer : public INrpProject
{
public:
	CNrpRetailer(const NrpText& name);
	~CNrpRetailer(void);

	static NrpText ClassName();
};

typedef CNrpRetailer* PNrpRetailer;

}//namespace nrp