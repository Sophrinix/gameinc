#include "StdAfx.h"
#include "NrpLicense.h"

namespace nrp
{

CNrpLicense::CNrpLicense(std::string name) : INrpProject( "CNrpLicense", "" )
{
	CreateValue<std::string>( NAME, name );
}

CNrpLicense::~CNrpLicense(void)
{
}

}//namespace nrp