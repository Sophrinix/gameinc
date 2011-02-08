#pragma once
#include "nrpConfig.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME IMAGESNUMBER( "imagesNumber" );
OPTION_NAME IMAGESBOXNUMBER( "imagesBoxNumber" );

class CNrpGame;

class CNrpScreenshot : public INrpConfig
{
public:
	CNrpScreenshot( const NrpText& fileName );
	~CNrpScreenshot(void);

	const STRINGS& GetBoxImages() { return _imagesBoxPath; }
	const STRINGS& GetImages() { return _imagesPath; }

	int GetEqualeRating( CNrpGame* game );
	bool IsMyYear( int year );

	static NrpText ClassName();
private:
	void InitializeOptions_();
	CNrpScreenshot();
	CNrpScreenshot( const CNrpScreenshot& a );

	STRINGS _imagesPath;
	STRINGS _imagesBoxPath;
	STRINGS _genres;

	void Load( const NrpText& fileName );
	NrpText Save( const NrpText& fileName ) { return ""; };
	bool _AddImage( STRINGS& art, const NrpText& fileName );
};

}//end namespace nrp