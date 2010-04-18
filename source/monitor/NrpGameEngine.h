#pragma once
#include "INrpProject.h"

namespace nrp
{

OPTION_NAME AVGENRE_COUNT( "avGenreCount" );
OPTION_NAME TIME_ACTUAL( "timeActual" ); //актуальность движка... пересчитывается при выходе новых технологий
OPTION_NAME GENRE_MODULE_NUMBER( "genreModuleNumber" );
OPTION_NAME CODEVOLUME( "volumeCode" );

class CNrpGameEngine : public INrpProject
{
public:
	CNrpGameEngine( std::string name );
	void AddGenre( GENRE_TYPE typen );
	int GetGenreCount() { return avgenres_.size(); }
	GENRE_TYPE GetGenreType( int index );
	bool IsGenreAvailble( GENRE_TYPE typen );
	void Save( std::string saveFolder );
	void Load( std::string loadFolder );

	~CNrpGameEngine(void);
private:
	typedef std::map< GENRE_TYPE, int > GENRE_MAP;

	GENRE_MAP avgenres_;
};

typedef CNrpGameEngine* PNrpGameEngine;

}//namespace nrp