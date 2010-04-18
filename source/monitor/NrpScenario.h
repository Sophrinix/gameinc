#pragma once
#include "INrpProject.h"

namespace nrp
{

class CNrpScenario : public INrpProject
{
public:
	CNrpScenario( std::string name );
	~CNrpScenario(void);
};

typedef CNrpScenario* PNrpScenario;

}//namespace nrp