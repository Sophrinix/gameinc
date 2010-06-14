#pragma once
#include "nrpConfig.h"

namespace nrp
{

const std::string CLASS_IMAGEGAMELIST( "CNrpGameImageList" );

OPTION_NAME ISUSED( "isUsed" );
OPTION_NAME YEAR( "year" );
OPTION_NAME IMAGESNUMBER( "imagesNumber" );
OPTION_NAME IMAGESBOXNUMBER( "imagesBoxNumber" );

class CNrpGameImageList : public INrpConfig
{
	void InitializeOptions_();
	CNrpGameImageList();
	void Load_( char* file_name );
public:
	std::vector< std::string > imagesPath;
	std::vector< std::string > imagesBoxPath;

	void Save( const std::string& fileName );
	void Load( const std::string& fileName );

	CNrpGameImageList( const char* name );

	CNrpGameImageList( const CNrpGameImageList& a );

	~CNrpGameImageList(void);
};

}//end namespace nrp