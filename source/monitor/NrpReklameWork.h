#pragma once
#include "nrpConfig.h"

namespace nrp
{

CLASS_NAME CLASS_REKLAMEWORK( "CNrpReklameWork" );

OPTION_NAME MAXQUALITY( "maxQuality" );
OPTION_NAME TYPEOBJECT( "typeObject" );

class CNrpReklameWork : public INrpConfig
{
	void Save( std::string sectionName, std::string fileName ) {};
	void Load( std::string sectionName, std::string fileName ) {}; 

	void InitializeOptions_();

public:
	CNrpReklameWork( const std::string& typeName, const std::string& gameName );
	CNrpReklameWork( const std::string& fileName );

	void Update( const CNrpReklameWork* p );
	void BeginNewDay();

	void Load( const std::string& fileName );
	std::string Save( const std::string& saveFolder );

	CNrpReklameWork( CNrpReklameWork& p );
	~CNrpReklameWork(void);

	static std::string ClassName() { return CLASS_REKLAMEWORK; }
};

}//end namespace nrp