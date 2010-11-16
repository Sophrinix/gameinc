#pragma once
#include "nrpConfig.h"
#include <irrArray.h>

namespace nrp
{
class CNrpGameProject;
class CNrpCompany;
class CNrpDevelopGame;
class CNrpHistory;

OPTION_NAME CASH( "cash" );
OPTION_NAME COPYSELL( "copySell" );
OPTION_NAME STARTRATING( "startRating" );
OPTION_NAME STARTGAMERATING( "startGameRating" );
OPTION_NAME STARTGRAPHICRATING( "startGraphicRating" );
OPTION_NAME STARTGENRERATING( "startGenreRating" );
OPTION_NAME STARTSOUNDRATING( "startSoundRating" );
OPTION_NAME STARTBUGRATING( "startBugRating" );
OPTION_NAME CURRENTGAMERATING( "currentGameRating" );
OPTION_NAME CURRENTGRAPHICRATING( "currentGraphicRating" );
OPTION_NAME CURRENTGENRERATING( "currentGenreRating" );
OPTION_NAME CURRENTSOUNDRATING( "currentSoundRating" );
OPTION_NAME LOCALIZATIONRATING( "localizationRating" );
OPTION_NAME CURRENTBUGRATING( "currentBugRating" );
OPTION_NAME STARTADVFUNCRATING( "startAdvFuncRating" );
OPTION_NAME CURRENTADVFUNCRATING( "currentAdvFuncRating" );
OPTION_NAME GBOX( "box" );
OPTION_NAME GAMEISSALING( "gameIsSaling" );
OPTION_NAME GAMEIMAGELIST( "gameImageList" );
OPTION_NAME VIEWIMAGE( "viewImage" );
OPTION_NAME GAMERETAILER( "gameRetailer" );

class CNrpGame : public INrpConfig
{
public:
	typedef irr::core::array< NrpText > STRINGS;
public:
	CNrpGame( const NrpText& fileName );
	~CNrpGame(void);

	CNrpGame( CNrpDevelopGame* devGame, CNrpCompany* ptrCompany );

	NrpText GetTechName( size_t index );
	NrpText GetGenreName( size_t index );
	bool IsGenreAvaible( const NrpText& name );

	float GetAuthorFamous();

	CNrpHistory* GetHistory();
	void GameBoxSaling( int number );

	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& loadFolder );

	static NrpText ClassName();
private:
	void InitializeOptions_();
	CNrpGame();;
	
	STRINGS _developers;
	STRINGS _genres;
	STRINGS _techs;
	CNrpHistory*  _history;
};

typedef CNrpGame* PNrpGame;
}//namespace nrp