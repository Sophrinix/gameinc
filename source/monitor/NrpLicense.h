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
	std::string Save( const std::string& pathTo );
	void Load( const std::string& pathTo );

	static std::string ClassName() { return CLASS_LICENSE; }
};

typedef CNrpLicense* PNrpLicense;

}//namespace nrp