#pragma once
#include "INrpProject.h"

namespace nrp
{

const std::string CLASS_NRPRETAILER( "CNrpRetailer" );

class CNrpRetailer : public INrpProject
{
public:
	CNrpRetailer(std::string name);
	~CNrpRetailer(void);
};

typedef CNrpRetailer* PNrpRetailer;

}//namespace nrp