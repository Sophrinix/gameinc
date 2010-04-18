#pragma once
#include "nrpConfig.h"

namespace nrp
{

OPTION_NAME STARTDATE( "startDate" );
OPTION_NAME ENDDATE( "endDate" );
OPTION_NAME MONEYONDEVELOP( "moneyDevelop" );
OPTION_NAME CASH( "cash" );
OPTION_NAME COPYSELL( "copySell" );
OPTION_NAME STARTRATING( "startRating" );
OPTION_NAME CURRENTRATING( "currentRating" );

class CNrpGame : public INrpConfig
{
public:
	CNrpGame( std::string name );
	~CNrpGame(void);
	void Save( std::string saveFolder );
	void Load( std::string loadFolder );
private:
	void Load_( char* name ) {}

	typedef std::map< std::string, std::string > DEVELOP_MAP;
	DEVELOP_MAP developers_;
};

typedef CNrpGame* PNrpGame;
}//namespace nrp