#pragma once
#include "nrpConfig.h"

namespace nrp
{
OPTION_NAME IMAGESNUMBER( "imagesNumber" );
OPTION_NAME IMAGESBOXNUMBER( "imagesBoxNumber" );

class CNrpGame;

class CNrpScreenshot : public INrpConfig
{
public:
	typedef core::array< NrpText > STRING_LIST;

	CNrpScreenshot( const NrpText& fileName );
	~CNrpScreenshot(void);

	const STRING_LIST& GetBoxImages() { return _imagesBoxPath; }
	const STRING_LIST& GetImages() { return _imagesPath; }

	int GetEqualeRating( CNrpGame* game );
	bool IsMyYear( int year );

	static NrpText ClassName();
private:
	void InitializeOptions_();
	CNrpScreenshot();
	CNrpScreenshot( const CNrpScreenshot& a );

	STRING_LIST _imagesPath;
	STRING_LIST _imagesBoxPath;
	STRING_LIST _genres;

	void Load( const NrpText& fileName );
	NrpText Save( const NrpText& fileName ) { return ""; };
};

}//end namespace nrp