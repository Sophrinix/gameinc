#pragma once
#include "nrpConfig.h"
#include <irrArray.h>

namespace nrp
{
class CNrpGameProject;
class CNrpCompany;
class CNrpDevelopGame;
class CNrpHistory;

OPTION_NAME CASH( L"cash" );
OPTION_NAME COPYSELL( L"copySell" );
OPTION_NAME STARTRATING( L"startRating" );
OPTION_NAME STARTGAMERATING( L"startGameRating" );
OPTION_NAME STARTGRAPHICRATING( L"startGraphicRating" );
OPTION_NAME STARTGENRERATING( L"startGenreRating" );
OPTION_NAME STARTSOUNDRATING( L"startSoundRating" );
OPTION_NAME STARTBUGRATING( L"startBugRating" );
OPTION_NAME CURRENTGAMERATING( L"currentGameRating" );
OPTION_NAME CURRENTGRAPHICRATING( L"currentGraphicRating" );
OPTION_NAME CURRENTGENRERATING( L"currentGenreRating" );
OPTION_NAME CURRENTSOUNDRATING( L"currentSoundRating" );
OPTION_NAME LOCALIZATIONRATING( L"localizationRating" );
OPTION_NAME CURRENTBUGRATING( L"currentBugRating" );
OPTION_NAME STARTADVFUNCRATING( L"startAdvFuncRating" );
OPTION_NAME CURRENTADVFUNCRATING( L"currentAdvFuncRating" );
OPTION_NAME GBOX( L"box" );
OPTION_NAME GAMEISSALING( L"gameIsSaling" );
OPTION_NAME GAMEIMAGELIST( L"gameImageList" );
OPTION_NAME VIEWIMAGE( L"viewImage" );
OPTION_NAME GAMERETAILER( L"gameRetailer" );

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