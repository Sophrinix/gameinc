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

void CNrpLicense::Load( const std::string& pathTo )
{

}

std::string CNrpLicense::Save( const std::string& pathTo )
{
	return "";
}

}//namespace nrp