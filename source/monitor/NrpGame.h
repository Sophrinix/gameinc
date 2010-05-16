#pragma once
#include "nrpConfig.h"

namespace nrp
{

class CNrpGameProject;
class CNrpCompany;

OPTION_NAME MONEYONDEVELOP( "moneyDevelop" );
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
OPTION_NAME STARTADVFUNC( "startAdvFunc" );
OPTION_NAME CURRENTADVFUNC( "currentAdvFunc" );
OPTION_NAME GAMEBOX( "gameBox" );

class CNrpGame : public INrpConfig
{
public:
	CNrpGame( std::string name );
	~CNrpGame(void);

	CNrpGame( CNrpGameProject* ptrProject, CNrpCompany* ptrCompany );

	std::string GetVideoTech( int index ) { return videoTechs_.at( index ); }
	std::string GetGenreTech( int index ) { return genreTechs_.at( index ); }
	std::string GetSoundTech( int index ) { return soundTechs_.at( index ); }
	std::string GetAdvTech( int index ) { return advTechs_.at( index ); }

	void Save( std::string saveFolder );
	void Load( std::string loadFolder );
private:
	void Load_( char* name ) {}
	void InitializeOptions_();
	typedef std::vector< std::string > DEVELOPERS_LIST;
	typedef std::vector< std::string > TECH_LIST;
	DEVELOPERS_LIST developers_;
		TECH_LIST	videoTechs_;
	TECH_LIST	genreTechs_;
	TECH_LIST	soundTechs_;
	TECH_LIST	advTechs_;
};

typedef CNrpGame* PNrpGame;
}//namespace nrp