#pragma once
#include "INrpProject.h"

namespace nrp
{

class CNrpLicense : public INrpProject
{
public:
	CNrpLicense(std::string name);
	~CNrpLicense(void);
};

typedef CNrpLicense* PNrpLicense;

}//namespace nrp