#pragma once
#include "INrpProject.h"

namespace nrp
{
CLASS_NAME CLASS_SCENARIO( "CNrpScenario" );

class CNrpScenario : public INrpProject
{
public:
	CNrpScenario( std::string name );
	~CNrpScenario(void);

	static std::string ClassName() { return CLASS_SCENARIO; }
	virtual std::string ObjectName() { return CLASS_SCENARIO; }
};

typedef CNrpScenario* PNrpScenario;

}//namespace nrp