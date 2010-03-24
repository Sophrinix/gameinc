#pragma once
#include "nrpConfig.h"

namespace nrp
{

OPTION_NAME CODE_SPEED( "codeSpeed" ); /*< How many string code may write developer in day */
OPTION_NAME CODE_QUALITY( "codeQuality" );/*< What quality we have on game end.Percent */
OPTION_NAME KNOWLEDGE_LEVEL( "knowledgeLevel" );/*< ”ровень знаний. ќт этого параметра зависит скорость роста скорости написани€ кода*/
OPTION_NAME TALANT( "talant" ); /*< “алант. от этого параметра зависит скорость роста качества кода*/
OPTION_NAME STAMINA( "stamina" );/*< ”сталость. ѕри значении ниже 90% умножаетс€ на codeSpeed_, дл€ получени€ конечного значени€ */
OPTION_NAME MOOD( "mood" );/*< Ќастроение. ѕри низком настроении может уволитьс€ */
OPTION_NAME POPULARITY( "popularity" );/*< изветность разработчика */
OPTION_NAME SALARY( "salary" );/*< зарплата разработчика*/
OPTION_NAME STABILITY("stability");/*< скорость падени€ усталости */

class IUser : public INrpConfig
{
public:
	IUser(const char* className, const char* systemName );
	~IUser(void);
private:         			
	void Load_( char* file_name ) {}
	std::map< std::string, int > genrePreferences_; /*< предпочтени€ в жанре */
	std::map< std::string, int > genreExperience_;  /*< опыт написани€ игр*/
	std::map< std::string, int > knowledges_;		/*< уровень знани€ технологий */
};

typedef IUser* PUser;

}//namespace nrp