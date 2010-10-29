#pragma once
#include "nrpConfig.h"

namespace nrp
{

CLASS_NAME CLASS_IMAGEGAMELIST( "CNrpGameImageList" );

OPTION_NAME IMAGESNUMBER( "imagesNumber" );
OPTION_NAME IMAGESBOXNUMBER( "imagesBoxNumber" );

class CNrpGame;

class CNrpScreenshot : public INrpConfig
{
	void InitializeOptions_();
	CNrpScreenshot();
	CNrpScreenshot( const CNrpScreenshot& a );

	typedef std::vector< std::string > STRING_LIST;
	STRING_LIST _imagesPath;
	STRING_LIST _imagesBoxPath;
	STRING_LIST _genres;

	void Load( const std::string& fileName );
	std::string Save( const std::string& fileName ) { return ""; };
public:
	CNrpScreenshot( const std::string& fileName );
	~CNrpScreenshot(void);

	const std::vector< std::string >& GetBoxImages() { return _imagesBoxPath; }
	const std::vector< std::string >& GetImages() { return _imagesPath; }

	int GetEqualeRating( CNrpGame* game );
	bool IsMyYear( int year );

	static std::string ClassName() { return CLASS_IMAGEGAMELIST; }
};

}//end namespace nrp