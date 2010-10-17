#pragma once
#include "INrpProject.h"

namespace nrp
{
CLASS_NAME CLASS_LICENSE( "CNrpLicense" );

class CNrpLicense : public INrpProject
{
public:
	CNrpLicense(std::string name);
	~CNrpLicense(void);

	static std::string ClassName() { return CLASS_LICENSE; }
	virtual std::string ObjectName() { return CLASS_LICENSE; }
};

typedef CNrpLicense* PNrpLicense;

}//namespace nrp