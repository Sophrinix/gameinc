#pragma once

#include <IEventReceiver.h>
#include "INrpObject.h"

class CNrpEngine;

nrp::CLASS_NAME CLASS_EVENTRECEIVER( "CNrpAppEventer" );

class CNrpAppEventer : public irr::IEventReceiver, public nrp::INrpObject
{
public:
	virtual bool OnEvent(const irr::SEvent& event);

	CNrpAppEventer( CNrpEngine* ptr_app );					//конструктор;

	static std::string ClassName() { return CLASS_EVENTRECEIVER; }
private:
	CNrpEngine* ptr_app_;
};
//////////////////////////////////////////////////////////////////////////