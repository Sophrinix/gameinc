#pragma once
#include "nrpConfig.h"

namespace nrp
{
CLASS_NAME CLASS_NRPGAME( "CNrpGame" );

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
	typedef std::vector< std::string > STRINGS;
public:
	CNrpGame( const std::string& fileName );
	~CNrpGame(void);
	CNrpGame( CNrpDevelopGame* devGame, CNrpCompany* ptrCompany );

	std::string GetTechName( size_t index );
	std::string GetGenreName( size_t index );
	bool IsGenreAvaible( const std::string& name );

	float GetAuthorFamous();

	CNrpHistory* GetHistory() { return history_; }
	void GameBoxSaling( int number );

	std::string Save( const std::string& saveFolder );
	void Load( const std::string& loadFolder );

	static std::string ClassName() { return CLASS_NRPGAME; }
private:
	void InitializeOptions_();
	CNrpGame() : INrpConfig( CLASS_NRPGAME, "" ) {};
	
	STRINGS developers_;
	STRINGS genres_;
	STRINGS techs_;
	CNrpHistory*  history_;
};

typedef CNrpGame* PNrpGame;
}//namespace nrp