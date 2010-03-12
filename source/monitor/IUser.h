#pragma once
#include "nrpConfig.h"

namespace nrp
{

const std::string USER_NAME( "name" );
const std::string CODE_SPEED( "codeSpeed" ); /*< How many string code may write developer in day */
const std::string CODE_QUALITY( "codeQuality" );/*< What quality we have on game end.Percent */
const std::string KNOWLEDGE_LEVEL( "knowledgeLevel" );/*< Уровень знаний. От этого параметра зависит скорость роста скорости написания кода*/
const std::string TALANT( "talant" ); /*< Талант. от этого параметра зависит скорость роста качества кода*/
const std::string STAMINA( "stamina" );/*< Усталость. При значении ниже 90% умножается на codeSpeed_, для получения конечного значения */
const std::string MOOD( "mood" );/*< Настроение. При низком настроении может уволиться */
const std::string POPULARITY( "popularity" );/*< изветность разработчика */
const std::string SALARY( "salary" );/*< зарплата разработчика*/
const std::string STABILITY("stability");/*< скорость падения усталости */

class IUser : public INrpConfig
{
public:
	IUser(void);
	~IUser(void);
private:         			
	std::map< std::string, int > genrePreferences_; /*< предпочтения в жанре */
	std::map< std::string, int > genreExperience_;  /*< опыт написания игр*/		
};

}//namespace nrp