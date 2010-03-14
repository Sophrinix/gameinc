#pragma once
#include "nrpConfig.h"

namespace nrp
{

OPTION_NAME CODE_SPEED( "codeSpeed" ); /*< How many string code may write developer in day */
OPTION_NAME CODE_QUALITY( "codeQuality" );/*< What quality we have on game end.Percent */
OPTION_NAME KNOWLEDGE_LEVEL( "knowledgeLevel" );/*< Уровень знаний. От этого параметра зависит скорость роста скорости написания кода*/
OPTION_NAME TALANT( "talant" ); /*< Талант. от этого параметра зависит скорость роста качества кода*/
OPTION_NAME STAMINA( "stamina" );/*< Усталость. При значении ниже 90% умножается на codeSpeed_, для получения конечного значения */
OPTION_NAME MOOD( "mood" );/*< Настроение. При низком настроении может уволиться */
OPTION_NAME POPULARITY( "popularity" );/*< изветность разработчика */
OPTION_NAME SALARY( "salary" );/*< зарплата разработчика*/
OPTION_NAME STABILITY("stability");/*< скорость падения усталости */

class IUser : public INrpConfig
{
public:
	IUser(const char* className, const char* systemName );
	~IUser(void);
private:         			
	void Load_( char* file_name ) {}
	std::map< std::string, int > genrePreferences_; /*< предпочтения в жанре */
	std::map< std::string, int > genreExperience_;  /*< опыт написания игр*/		
};

}//namespace nrp