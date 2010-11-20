#ifndef NRP_USER_H
#define NRP_USER_H

#include "nrpConfig.h"
#include "nrpArrays.h"
#include "NrpTypes.h"

namespace nrp
{
OPTION_NAME CODE_SPEED( L"codeSpeed" ); /*< How many string code may write developer in day */
OPTION_NAME CODE_QUALITY( L"codeQuality" );/*< What quality we have on game end.Percent */
OPTION_NAME KNOWLEDGE_LEVEL( L"knowledgeLevel" );/*< Уровень знаний. От этого параметра зависит скорость роста скорости написания кода*/
OPTION_NAME TALANT( L"talant" ); /*< Талант. от этого параметра зависит скорость роста качества кода*/
OPTION_NAME STAMINA( L"stamina" );/*< Усталость. При значении ниже 90% умножается на codeSpeed_, для получения конечного значения */
OPTION_NAME MAXMOOD( L"maxMood" );/*! Максимальный уровень настроения, рассчитывается в начале дня */
OPTION_NAME MOOD( L"mood" );/*< Настроение. При низком настроении может уволиться */
OPTION_NAME POPULARITY( L"popularity" );/*< изветность разработчика */
OPTION_NAME SALARY( L"salary" );/*< зарплата разработчика*/
OPTION_NAME WANTMONEY( L"wantMoney" ); /*! Зарплата, которой будет доволен работник */
OPTION_NAME STABILITY( L"stability");/*< скорость падения усталости */
OPTION_NAME CHARACTER( L"character" ); /*< характер персонажа */
OPTION_NAME CONTRACTMONEY( L"contractMoney" ); /*! стартовая сумма при подписании контракта*/
OPTION_NAME MAXSUPPORTUSER( L"maxSupportUser" ); /*! количество людей, которым может управлять чел*/
OPTION_NAME INVENTIONABILITY( L"inventionAbility" ); /*! способность к изобретениям*/
OPTION_NAME USERSTATE( L"userState" ); /*! Состояние пользователя "работа", "отдых" */
OPTION_NAME ROOMSTATE( L"roomState" ); /*! Комната, в которой находится человек */
OPTION_NAME MAXHOLIDAY( L"maxHoliday" );	 /*! Максимальное количество дней отпуска для этого человека */
OPTION_NAME CURRENTHOLIDAY( L"currentHoliday" ); /*! Количество отгулянных дней отпуска */
OPTION_NAME HANGRY( L"hangry" ); /*! Чуство голода*/
OPTION_NAME COLLECTIVINTEGRATION( L"collectivintegration" ); /*! Умение работать в коллективе*/
OPTION_NAME TIMEMANAGMENT( L"timeManagment" ); /*!  Управление временем, от этого параметра зависит реальное время потраченное на разарботку */
OPTION_NAME EXPERIENCE( L"experience" );

class IUserAction;
class CNrpTechnology; 
class CNrpCompany;
class IModificator;
class IWorkingModule;
class CNrpRelation;

class IUser : public INrpConfig
{
public:
	IUser(const NrpText& className, const NrpText& systemName );

	void SetSkill( int typen, int valuel ); 
	void SetSkill( const NrpText& name, int valuel );
	int GetSkill( int typen );
	
	int GetGenreExperience( GENRE_TYPE typen );
	void SetGenreExperience( GENRE_TYPE typen, int valuel );
	void IncreaseExperience( int techGroup, int grow );

	int GetGenrePreferences( GENRE_TYPE typen );
	void SetGenrePreferences( GENRE_TYPE typen, int valuel );
	
	void AddWork( IWorkingModule* techWork, bool toFront=false );

	IWorkingModule* GetWork( u32 index ) const;
	IWorkingModule* GetWork( const NrpText& name ) const;
	void RemoveWork( IWorkingModule* techWork );

	void AddModificator( IModificator* ptrModificator );

	void BeginNewHour( const SYSTEMTIME& time );
	void BeginNewDay( const SYSTEMTIME& time );

	CNrpRelation* GetRelation( const NrpText& name );

	~IUser(void);
	NrpText Save( const NrpText& folderPath );
	void Load( const NrpText& fileName );

	bool Equale( const NrpText& name );

	static NrpText ClassName();

private:         			
	void CalculateWantSalary_();
	void CalculateKnowledgeLevel_();

	void RemoveOldModificators_( const SYSTEMTIME& time );
	void CheckModificators_();

	KNOWLEDGE_MAP genrePreferences_; /*< предпочтения в жанре, растут рандомно со временем */
	KNOWLEDGE_MAP genreExperience_;  /*< опыт написания игр, растет по мере выполнения компонентов */
	KNOWLEDGE_MAP knowledges_;		/*< уровень знания технологий */
	WORKS works_;
	MODIFICATORS modificators_;
	RELATION_MAP _relations;	
};

typedef IUser* PUser;

}//namespace nrp

#endif