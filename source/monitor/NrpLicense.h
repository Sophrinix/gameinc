#pragma once
#include "INrpProject.h"

namespace nrp
{
class CNrpLicense : public INrpProject
{
public:
	CNrpLicense(const NrpText& name);
	~CNrpLicense(void);
	NrpText Save( const NrpText& pathTo );
	void Load( const NrpText& pathTo );

	static NrpText ClassName();
};

typedef CNrpLicense* PNrpLicense;

}//namespace nrp