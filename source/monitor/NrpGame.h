#pragma once
#include "nrpConfig.h"
#include <irrArray.h>

namespace nrp
{
class CNrpGameProject;
class CNrpCompany;
class CNrpDevelopGame;
class CNrpHistory;
class CNrpExtInfo;

OPTION_NAME CASH( L"cash" );
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
OPTION_NAME EXTINFO( L"gameImageList" );
OPTION_NAME EXTINFOLINK( L"extInfoLink" );
OPTION_NAME VIEWIMAGE( L"viewImage" );
OPTION_NAME GAMERETAILER( L"gameRetailer" );
OPTION_NAME RECENSE( L"recense" );
OPTION_NAME NPC_GAME( L"npcGame" );
OPTION_NAME BESTSALER( L"bestsaler" );
OPTION_NAME OLDGAME( L"oldgame" );

class NrpTime;

class CNrpGame : public INrpConfig
{
public:
	typedef irr::core::array< NrpText > STRINGS;
public:
    static const NrpText saveTemplate;
    static const NrpText historyTemplate;
    static const NrpText extension;

	CNrpGame( const NrpText& fileName );
	~CNrpGame(void);

	CNrpGame( const CNrpDevelopGame& devGame, CNrpCompany* ptrCompany );

	NrpText GetTechName( size_t index );
	NrpText GetGenreName( size_t index );
    NrpText GetPlatformName( size_t index );
	bool IsGenreAvaible( const NrpText& name ) const;

	float GetAuthorFamous();

	CNrpHistory* GetHistory();
	void AddSales( int number, NrpTime& curTime, int price=0 );

	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& loadFolder );

	static NrpText ClassName();
private:
	void _InitializeOptions();
	CNrpExtInfo* _SearchExtInfo();
    void _AddSalesToCompany( int number, int profit, NrpTime& curTime );

	CNrpGame();
    
	STRINGS _developers;
	STRINGS _genres;
	STRINGS _techs;
	STRINGS _platforms;
	CNrpHistory*  _history;
};

typedef CNrpGame* PNrpGame;

}//namespace nrp