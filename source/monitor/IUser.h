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
OPTION_NAME WANTMONEY( "wantMoney" ); /*! «арплата, которой будет доволен юзер */
OPTION_NAME STABILITY("stability");/*< скорость падени€ усталости */
OPTION_NAME CHARACTER( "character" ); /*< характер персонажа */
OPTION_NAME CONTRACTMONEY( "contractMoney" ); /*! стартова€ сумма при подписании контракта*/
OPTION_NAME MAXSUPPORTUSER( "maxSupportUser" ); /*! количество людей, которым может управл€ть чел*/
OPTION_NAME INVENTIONABILITY( "inventionAbility" ); /*! способность к изобретени€м*/
OPTION_NAME USERSTATE( "userState" ); /*! —осто€ние пользовател€ "работа", "отдых" */
OPTION_NAME ROOMSTATE( "roomState" ); /*!  омната, в которой находитс€ человек */
OPTION_NAME PARENTCOMPANY( "parentCompany" );
 
class IUserAction;
class CNrpTechnology;
class CNrpCompany;

class IUser : public INrpConfig
{
public:
	IUser(const char* className, const char* systemName, CNrpCompany* ptrCmp );
	void SetSkill( int typen, int valuel ); 
	void SetSkill( std::string name, int valuel );
	int GetSkill( int typen );
	int GetGenreExperience( int typen );
	int GetGenrePreferences( int typen );
	void AddTechWork( CNrpTechnology* techWork );
	CNrpTechnology* GetTechWork( int index );
	void RemoveTechWork( CNrpTechnology* techWork );
	void Update( const SYSTEMTIME& time );
	~IUser(void);
	void Save( std::string folderPath );
	void Load( std::string fileName );

private:         			
	void Load_( char* file_name ) {}
	void CalculateWantSalary_();
	void CalculateKnowledgeLevel_();

	typedef std::map< int, int > KNOWLEDGE_MAP;
	typedef std::vector< CNrpTechnology* > TECH_LIST;
	typedef std::vector< IUserAction* > USERACTION_LIST;

	KNOWLEDGE_MAP genrePreferences_; /*< предпочтени€ в жанре, растут рандомно со временем */
	KNOWLEDGE_MAP genreExperience_;  /*< опыт написани€ игр, растет по мере выполнени€ компонентов */
	KNOWLEDGE_MAP knowledges_;		/*< уровень знани€ технологий */
	USERACTION_LIST peopleFeels_; /*< ќтношени€ с окружающими людьми */
	TECH_LIST techWorks_;
};

typedef IUser* PUser;

}//namespace nrp