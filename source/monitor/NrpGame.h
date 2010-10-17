#pragma once
#include "nrpConfig.h"

namespace nrp
{
CLASS_NAME CLASS_NRPGAME( "CNrpGame" );

class CNrpGameProject;
class CNrpCompany;
class CNrpDevelopGame;

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
OPTION_NAME IMAGENAME( "imageName" );
OPTION_NAME GAMEIMAGELIST( "gameImageList" );
OPTION_NAME VIEWIMAGE( "viewImage" );
OPTION_NAME GAMERETAILER( "gameRetailer" );

class CNrpHistoryStep
{
	CNrpHistoryStep() {}
public:
	int numberSale;
	int month;
	int cash;
	CNrpHistoryStep( int m, int n, int c) : month(m), numberSale(n), cash(c)
	{}

	void Add( int n, int c )
	{
		numberSale += n;
		cash += c;
	}
};

class CNrpGame : public INrpConfig
{
public:
	typedef std::vector< std::string > STRINGS;
	typedef std::map< int, CNrpHistoryStep* > SALE_HISTORY_MAP;
public:
	CNrpGame( std::string name );
	~CNrpGame(void);
	CNrpGame( CNrpDevelopGame* devGame, CNrpCompany* ptrCompany );

	std::string GetTechName( size_t index );
	std::string GetGenreName( size_t index );
	float GetAuthorFamous();

	const SALE_HISTORY_MAP& GetSalesHistory() { return history_; }
	void GameBoxSaling( int number );

	void Save( std::string saveFolder );
	void Load( std::string loadFolder );

	static std::string ClassName() { return CLASS_NRPGAME; }
	virtual std::string ObjectName() { return CLASS_NRPGAME; }
private:
	void Load_( char* name ) {}
	void InitializeOptions_();
	
	STRINGS developers_;
	STRINGS genres_;
	STRINGS techs_;
	SALE_HISTORY_MAP history_;
};

typedef CNrpGame* PNrpGame;
}//namespace nrp