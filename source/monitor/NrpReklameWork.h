#pragma once
#include "nrpConfig.h"

const std::string CLASS_REKLAMEWORK( "CNrpReklameWork" );

namespace nrp
{

OPTION_NAME MAXQUALITY( "maxQuality" );

class CNrpReklameWork : public INrpConfig
{
	void Load_( char* file_name ) {};
	void InitializeOptions_();

public:
	CNrpReklameWork( std::string typeName, 
					 std::string gameName );

	void Update( CNrpReklameWork* p );
	void BeginNewDay();
	void Load( std::string sectionName, std::string fileName );
	CNrpReklameWork( CNrpReklameWork& p );
	~CNrpReklameWork(void);
};

}//end namespace nrp