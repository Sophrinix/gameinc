#pragma once
#include "nrpConfig.h"
#include "NrpUserModificator.h"

namespace nrp
{

OPTION_NAME SECTION_WORKS("works");

OPTION_NAME CODE_SPEED( "codeSpeed" ); /*< How many string code may write developer in day */
OPTION_NAME CODE_QUALITY( "codeQuality" );/*< What quality we have on game end.Percent */
OPTION_NAME KNOWLEDGE_LEVEL( "knowledgeLevel" );/*< Уровень знаний. От этого параметра зависит скорость роста скорости написания кода*/
OPTION_NAME TALANT( "talant" ); /*< Талант. от этого параметра зависит скорость роста качества кода*/
OPTION_NAME STAMINA( "stamina" );/*< Усталость. При значении ниже 90% умножается на codeSpeed_, для получения конечного значения */
OPTION_NAME MAXMOOD( "maxMood" );/*! Максимальный уровень настроения, рассчитывается в начале дня */
OPTION_NAME MOOD( "mood" );/*< Настроение. При низком настроении может уволиться */
OPTION_NAME POPULARITY( "popularity" );/*< изветность разработчика */
OPTION_NAME SALARY( "salary" );/*< зарплата разработчика*/
OPTION_NAME WANTMONEY( "wantMoney" ); /*! Зарплата, которой будет доволен работник */
OPTION_NAME STABILITY("stability");/*< скорость падения усталости */
OPTION_NAME CHARACTER( "character" ); /*< характер персонажа */
OPTION_NAME CONTRACTMONEY( "contractMoney" ); /*! стартовая сумма при подписании контракта*/
OPTION_NAME MAXSUPPORTUSER( "maxSupportUser" ); /*! количество людей, которым может управлять чел*/
OPTION_NAME INVENTIONABILITY( "inventionAbility" ); /*! способность к изобретениям*/
OPTION_NAME USERSTATE( "userState" ); /*! Состояние пользователя "работа", "отдых" */
OPTION_NAME ROOMSTATE( "roomState" ); /*! Комната, в которой находится человек */
OPTION_NAME MAXHOLIDAY( "maxHoliday" );	 /*! Максимальное количество дней отпуска для этого человека */
OPTION_NAME CURRENTHOLIDAY( "currentHoliday" ); /*! Количество отгулянных дней отпуска */
OPTION_NAME HANGRY( "hangry" ); /*! Чуство голода*/
OPTION_NAME COLLECTIVINTEGRATION( "collectivintegration" ); /*! Умение работать в коллективе*/
OPTION_NAME TIMEMANAGMENT( "timeManagment" ); /*!  Управление временем, от этого параметра зависит реальное время потраченное на разарботку */
OPTION_NAME EXPERIENCE( "experience" );

class IUserAction;
class CNrpTechnology; 
class CNrpCompany;
class IModificator;
class IWorkingModule;
class CNrpRelation;

class IUser : public INrpConfig
{
public:
	IUser(const std::string className, const std::string systemName );
	void SetSkill( int typen, int valuel ); 
	void SetSkill( std::string name, int valuel );
	int GetSkill( int typen );
	
	int GetGenreExperience( int typen );
	void SetGenreExperience( int typen, int valuel );
	void IncreaseExperience( int techGroup, int grow );

	int GetGenrePreferences( int typen );
	void SetGenrePreferences( int typen, int valuel );
	
	void AddWork( IWorkingModule* techWork );

	IWorkingModule* GetWork( int index );
	IWorkingModule* GetWork( std::string name );
	void RemoveWork( IWorkingModule* techWork );

	void AddModificator( IModificator* ptrModificator );

	void BeginNewHour( const SYSTEMTIME& time );
	void BeginNewDay( const SYSTEMTIME& time );

	CNrpRelation* GetRelation( const std::string& name );

	~IUser(void);
	void Save( std::string folderPath );
	void Load( std::string fileName );

private:         			
	void Load_( char* file_name ) {}
	void CalculateWantSalary_();
	void CalculateKnowledgeLevel_();

	void RemoveOldModificators_( const SYSTEMTIME& time );
	void CheckModificators_();
	typedef std::map< int, int > KNOWLEDGE_MAP;
	typedef std::vector< IWorkingModule* > WORK_LIST;
	typedef std::vector< IUserAction* > USERACTION_LIST;
	typedef std::vector< IModificator* > MODIFICATOR_LIST;
	typedef std::map< std::string, CNrpRelation* > RELATION_MAP;

	KNOWLEDGE_MAP genrePreferences_; /*< предпочтения в жанре, растут рандомно со временем */
	KNOWLEDGE_MAP genreExperience_;  /*< опыт написания игр, растет по мере выполнения компонентов */
	KNOWLEDGE_MAP knowledges_;		/*< уровень знания технологий */
	USERACTION_LIST peopleFeels_; /*< Отношения с окружающими людьми */
	WORK_LIST works_;
	MODIFICATOR_LIST modificators_;
	RELATION_MAP _relations;	
};

typedef IUser* PUser;

}//namespace nrp