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
OPTION_NAME WORK_QUALITY_AWARD( L"codeQualityAward" ); /*< �������� ���������� �������� ���������*/
OPTION_NAME KNOWLEDGE_LEVEL( L"knowledgeLevel" );/*< ������� ������. �� ����� ��������� ������� �������� ����� �������� ��������� ����*/
OPTION_NAME TALANT( L"talant" ); /*< ������. �� ����� ��������� ������� �������� ����� �������� ����*/
OPTION_NAME STAMINA( L"stamina" );/*< ���������. ��� �������� ���� 90% ���������� �� codeSpeed_, ��� ��������� ��������� �������� */
OPTION_NAME MAXMOOD( L"maxMood" );/*! ������������ ������� ����������, �������������� � ������ ��� */
OPTION_NAME MOOD( L"mood" );/*< ����������. ��� ������ ���������� ����� ��������� */
OPTION_NAME SALARY( L"salary" );/*< �������� ������������*/
OPTION_NAME WANTMONEY( L"wantMoney" ); /*! ��������, ������� ����� ������� �������� */
OPTION_NAME STABILITY( L"stability");/*< �������� ������� ��������� */
OPTION_NAME CHARACTER( L"character" ); /*< �������� ��������� */
OPTION_NAME CONTRACTMONEY( L"contractMoney" ); /*! ��������� ����� ��� ���������� ���������*/
OPTION_NAME MAXSUPPORTUSER( L"maxSupportUser" ); /*! ���������� �����, ������� ����� ��������� ���*/
OPTION_NAME INVENTIONABILITY( L"inventionAbility" ); /*! ����������� � ������������*/
OPTION_NAME USERSTATE( L"userState" ); /*! ��������� ������������ "������", "�����" */
OPTION_NAME ROOMSTATE( L"roomState" ); /*! �������, � ������� ��������� ������� */
OPTION_NAME MAXHOLIDAY( L"maxHoliday" );	 /*! ������������ ���������� ���� ������� ��� ����� �������� */
OPTION_NAME CURRENTHOLIDAY( L"currentHoliday" ); /*! ���������� ���������� ���� ������� */
OPTION_NAME HANGRY( L"hangry" ); /*! ������ ������*/
OPTION_NAME COLLECTIVINTEGRATION( L"collectivintegration" ); /*! ������ �������� � ����������*/
OPTION_NAME TIMEMANAGMENT( L"timeManagment" ); /*!  ���������� ��������, �� ����� ��������� ������� �������� ����� ����������� �� ���������� */
OPTION_NAME EXPERIENCE( L"experience" );
OPTION_NAME ALL_SKILL_SUMM( L"allSkillSumm" );
OPTION_NAME LAST_AWARD( "lastAward" );
OPTION_NAME ALCOHOL("alcohol");					/*! ��������� � �������� */


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
	KNOWLEDGE_MAP genrePreferences_; /*< ������������ � �����, ������ �������� �� �������� */
	KNOWLEDGE_MAP genreExperience_;  /*< ���� ��������� ���, ������ �� ���� ���������� ����������� */
	KNOWLEDGE_MAP knowledges_;		/*< ������� ������ ���������� */
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