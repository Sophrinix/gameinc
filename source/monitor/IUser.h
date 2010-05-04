#pragma once
#include "nrpConfig.h"

namespace nrp
{

OPTION_NAME CODE_SPEED( "codeSpeed" ); /*< How many string code may write developer in day */
OPTION_NAME CODE_QUALITY( "codeQuality" );/*< What quality we have on game end.Percent */
OPTION_NAME KNOWLEDGE_LEVEL( "knowledgeLevel" );/*< Уровень знаний. От этого параметра зависит скорость роста скорости написания кода*/
OPTION_NAME TALANT( "talant" ); /*< Талант. от этого параметра зависит скорость роста качества кода*/
OPTION_NAME STAMINA( "stamina" );/*< Усталость. При значении ниже 90% умножается на codeSpeed_, для получения конечного значения */
OPTION_NAME MAXMOOD( "maxMood" );/*! Максимальный уровень настроения, рассчитывается в начале дня */
OPTION_NAME MOOD( "mood" );/*< Настроение. При низком настроении может уволиться */
OPTION_NAME POPULARITY( "popularity" );/*< изветность разработчика */
OPTION_NAME SALARY( "salary" );/*< зарплата разработчика*/
OPTION_NAME WANTMONEY( "wantMoney" ); /*! Зарплата, которой будет доволен юзер */
OPTION_NAME STABILITY("stability");/*< скорость падения усталости */
OPTION_NAME CHARACTER( "character" ); /*< характер персонажа */
OPTION_NAME CONTRACTMONEY( "contractMoney" ); /*! стартовая сумма при подписании контракта*/
OPTION_NAME MAXSUPPORTUSER( "maxSupportUser" ); /*! количество людей, которым может управлять чел*/
OPTION_NAME INVENTIONABILITY( "inventionAbility" ); /*! способность к изобретениям*/
OPTION_NAME USERSTATE( "userState" ); /*! Состояние пользователя "работа", "отдых" */
OPTION_NAME ROOMSTATE( "roomState" ); /*! Комната, в которой находится человек */
OPTION_NAME PARENTCOMPANY( "parentCompany" ); /*! компания в которой работаем человек */
OPTION_NAME MAXHOLIDAY( "maxHoliday" );	 /*! Максимальное количество дней отпуска для этого человека */
OPTION_NAME CURRENTHOLIDAY( "currentHoliday" ); /*! Количество отгулянных дней отпуска */
OPTION_NAME HANGRY( "hangry" ); /*! Чуство голода*/
OPTION_NAME COLLECTIVINTEGRATION( "collectivintegration" ); /*! Умение работать в коллективе*/
OPTION_NAME TIMEMANAGMENT( "timeManagment" ); /*!  Управление временем, от этого параметра зависит реальное время потраченное на разарботку */

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
	void SetGenreExperience( int typen, int valuel );

	int GetGenrePreferences( int typen );
	void SetGenrePreferences( int typen, int valuel );
	
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

	KNOWLEDGE_MAP genrePreferences_; /*< предпочтения в жанре, растут рандомно со временем */
	KNOWLEDGE_MAP genreExperience_;  /*< опыт написания игр, растет по мере выполнения компонентов */
	KNOWLEDGE_MAP knowledges_;		/*< уровень знания технологий */
	USERACTION_LIST peopleFeels_; /*< Отношения с окружающими людьми */
	TECH_LIST techWorks_;
};

typedef IUser* PUser;

}//namespace nrp