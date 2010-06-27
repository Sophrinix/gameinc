#pragma once
#include "NrpTechnology.h"

namespace nrp
{

class INrpProject;

class CNrpProjectModule : public CNrpTechnology
{
	void InitializeOptions_();
	CNrpProjectModule();
public:
	CNrpProjectModule(  CNrpTechnology* pTech, INrpProject* pProject  );
	CNrpProjectModule( PROJECT_TYPE type, INrpProject* pProject );
	~CNrpProjectModule(void);

	void SetLider( IUser* ptrUser );
	void Update( IUser* ptrUser );

	virtual void Save( std::string saveFolder );
	virtual void Load( std::string fileName );
};

typedef CNrpProjectModule* PNrpProjectModule;

}//end namespace nrp