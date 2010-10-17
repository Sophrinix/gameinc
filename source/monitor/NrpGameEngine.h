#pragma once
#include "INrpProject.h"

namespace nrp
{

CLASS_NAME CLASS_GAMEENGINE( "CNrpGameEngine" );

OPTION_NAME AVGENRE_COUNT( "avGenreCount" );
OPTION_NAME TIME_ACTUAL( "timeActual" ); //������������ ������... ��������������� ��� ������ ����� ����������
OPTION_NAME SKILL_CODING( "skillCoding" );

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

	static std::string ClassName() { return CLASS_GAMEENGINE; }
	virtual std::string ObjectName() { return CLASS_GAMEENGINE; }
private:
	typedef std::map< GENRE_TYPE, int > GENRE_MAP;

	GENRE_MAP avgenres_;
};

typedef CNrpGameEngine* PNrpGameEngine;

}//namespace nrp