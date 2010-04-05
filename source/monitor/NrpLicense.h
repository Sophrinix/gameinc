#pragma once
#include "INrpProject.h"

namespace nrp
{

class CNrpLicense : public INrpProject
{
public:
	CNrpLicense(void);
	~CNrpLicense(void);
};

typedef CNrpLicense* PNrpLicense;

}//namespace nrp