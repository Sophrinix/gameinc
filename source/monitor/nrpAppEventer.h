#pragma once

#include <IEventReceiver.h>
#include "INrpObject.h"

class CNrpEngine;

class CNrpAppEventer : public irr::IEventReceiver, public nrp::INrpObject
{
public:
	virtual bool OnEvent(const irr::SEvent& event);

	CNrpAppEventer( CNrpEngine* ptr_app );					//конструктор;

	static nrp::NrpText ClassName();
private:
	CNrpEngine* ptr_app_;
};
//////////////////////////////////////////////////////////////////////////