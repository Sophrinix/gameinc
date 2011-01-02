#pragma once
#include "INrpProject.h"

namespace nrp
{
class CNrpScenario : public INrpProject
{
public:
	CNrpScenario( const NrpText& saveFile );
	~CNrpScenario(void);

	NrpText Save( const NrpText& pathTo );
	void Load( const NrpText& pathTo );

	static NrpText ClassName();
};

typedef CNrpScenario* PNrpScenario;

}//namespace nrp