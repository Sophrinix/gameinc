#pragma once
#include "nrpconfig.h"
#include "nrpArrays.h"

namespace nrp
{

class CNrpBridge : public INrpConfig
{
private:
	CNrpBridge();

public:
	static CNrpBridge& Instance();
	static NrpText ClassName();

	void Update();
private:
	COMPANIES _companies;
	void CNrpBridge::_CheckCompany( CNrpCompany* company );

	float _GetMiddlePercent( const NrpText& name );
};

}//namespace nrp