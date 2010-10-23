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

	std::string Save( const std::string& pathTo );
	void Load( const std::string& pathTo );

	static std::string ClassName() { return CLASS_SCENARIO; }
};

typedef CNrpScenario* PNrpScenario;

}//namespace nrp