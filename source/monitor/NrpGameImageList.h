#pragma once
#include "nrpConfig.h"

namespace nrp
{

CLASS_NAME CLASS_IMAGEGAMELIST( "CNrpGameImageList" );

OPTION_NAME ISUSED( "isUsed" );
OPTION_NAME YEAR( "year" );
OPTION_NAME IMAGESNUMBER( "imagesNumber" );
OPTION_NAME IMAGESBOXNUMBER( "imagesBoxNumber" );

class CNrpGameImageList : public INrpConfig
{
	void InitializeOptions_();
	CNrpGameImageList();
public:
	std::vector< std::string > imagesPath;
	std::vector< std::string > imagesBoxPath;

	std::string Save( const std::string& fileName );
	void Load( const std::string& fileName );

	CNrpGameImageList( const char* name );
	CNrpGameImageList( const std::string& fileName, bool load );
	CNrpGameImageList( const CNrpGameImageList& a );

	~CNrpGameImageList(void);

	static std::string ClassName() { return CLASS_IMAGEGAMELIST; }
};

}//end namespace nrp