#pragma once
#include "INrpProject.h"

namespace nrp
{

CLASS_NAME CLASS_NRPRETAILER( "CNrpRetailer" );

class CNrpRetailer : public INrpProject
{
public:
	CNrpRetailer(std::string name);
	~CNrpRetailer(void);

	static std::string ClassName() { return CLASS_NRPRETAILER; }
};

typedef CNrpRetailer* PNrpRetailer;

}//namespace nrp