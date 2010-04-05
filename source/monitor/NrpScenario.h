#pragma once
#include "INrpProject.h"

namespace nrp
{

class CNrpScenario : public INrpProject
{
public:
	CNrpScenario(void);
	~CNrpScenario(void);
};

typedef CNrpScenario* PNrpScenario;

}//namespace nrp