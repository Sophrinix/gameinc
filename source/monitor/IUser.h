#ifndef NRP_USER_H
#define NRP_USER_H

#include "nrpConfig.h"
#include "nrpArrays.h"
#include "NrpTypes.h"
#include "NrpTime.h"

namespace nrp
{
OPTION_NAME WORK_SPEED( L"codeSpeed" ); /*< How many string code may write developer in day */
OPTION_NAME WORK_QUALITY( L"codeQuality" );/*< What quality we have on game end.Percent */
OPTION_NAME WORK_QUALITY_AWARD( L"codeQualityAward" ); /*< Значение ожидаемого прироста параметра*/
OPTION_NAME KNOWLEDGE_LEVEL( L"knowledgeLevel" );/*< Уровень знаний. От этого параметра зависит скорость роста скорости написания кода*/
OPTION_NAME TALANT( L"talant" ); /*< Талант. от этого параметра зависит скорость роста качества кода*/
OPTION_NAME STAMINA( L"stamina" );/*< Усталость. При значении ниже 90% умножается на codeSpeed_, для получения конечного значения */
OPTION_NAME MAXMOOD( L"maxMood" );/*! Максимальный уровень настроения, рассчитывается в начале дня */
OPTION_NAME MOOD( L"mood" );/*< Настроение. При низком настроении может уволиться */
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
OPTION_NAME ALL_SKILL_SUMM( L"allSkillSumm" );
OPTION_NAME LAST_AWARD( "lastAward" );
OPTION_NAME ALCOHOL("alcohol");					/*! отношение к алкоголю */


class IUserAction;
class CNrpTechnology; 
class CNrpCompany;
class IModificator;
class IWorkingModule;
class CNrpRelation;

class CNrpUser : public INrpConfig
{
	friend class CLuaUser;
public:
	CNrpUser(const NrpText& className, const NrpText& systemName );
	~CNrpUser(void);

	void SetSkill( const NrpText& name, int valuel ); 
	int GetSkill( const NrpText& name );
	
	int GetGenreExperience( const NrpText& name );
	void SetGenreExperience( const NrpText& name, int valuel );
	void IncreaseExperience( const NrpText& name, int grow );

	int GetGenrePreferences( const NrpText& name );
	void SetGenrePreferences( const NrpText& name, int valuel );
	
	virtual void AddWork( IWorkingModule& techWork, bool toFront=false );
	IWorkingModule* GetWork( u32 index ) const;
	IWorkingModule* GetWork( const NrpText& name ) const;
	void RemoveWork( IWorkingModule& techWork );

	void AddModificator( IModificator* ptrModificator );

	virtual void BeginNewHour( const NrpTime& time );
	virtual void BeginNewDay( const NrpTime& time );

	CNrpRelation* GetRelation( const NrpText& name );

	NrpText Save( const NrpText& folderPath );
	void Load( const NrpText& fileName );

	bool Equale( const NrpText& name );

	bool operator == ( const CNrpUser& other );

	static NrpText ClassName();

//notificators
	void CheckHangry( NParam& paramt );

protected:         			
	void CalculateWantSalary_();
	void CalculateKnowledgeLevel_();

	void RemoveOldModificators_( NrpTime time );
	void _CheckModificators();
	KNOWLEDGE_MAP genrePreferences_; /*< предпочтения в жанре, растут рандомно со временем */
	KNOWLEDGE_MAP genreExperience_;  /*< опыт написания игр, растет по мере выполнения компонентов */
	KNOWLEDGE_MAP knowledges_;		/*< уровень знания технологий */
	WORKS works_;
	MODIFICATORS modificators_;
	RELATION_MAP _relations;	
};

class NrpCoder : public CNrpUser
{
public:
	NrpCoder( const NrpText& systemName );

	static NrpText ClassName();
};

class NrpDesigner : public CNrpUser
{
public:
	NrpDesigner( const NrpText& systemName );

	static NrpText ClassName();
};

class NrpComposer : public CNrpUser
{
public:
	NrpComposer( const NrpText& systemName );

	static NrpText ClassName();
};


typedef CNrpUser* PUser;

}//namespace nrp

#endif