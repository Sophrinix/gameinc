#pragma once
#include "nrpConfig.h"

namespace nrp
{
OPTION_NAME MAXQUALITY( L"maxQuality" );
OPTION_NAME TYPEOBJECT( L"typeObject" );

class CNrpReklameWork : public INrpConfig
{
	void InitializeOptions_();

public:
	CNrpReklameWork( const NrpText& typeName, const NrpText& gameName );
	CNrpReklameWork( const NrpText& fileName );

	void Update( const CNrpReklameWork* p );
	void BeginNewDay();

	void Load( const NrpText& fileName );
	NrpText Save( const NrpText& saveFolder );
	bool Equale( const NrpText& type, const NrpText& gName );

	CNrpReklameWork( const CNrpReklameWork& p );
	~CNrpReklameWork(void);

	static NrpText ClassName();
};

}//end namespace nrp