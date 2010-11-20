#ifndef NRP_USER_H
#define NRP_USER_H

#include "nrpConfig.h"
#include "nrpArrays.h"
#include "NrpTypes.h"

namespace nrp
{
OPTION_NAME CODE_SPEED( L"codeSpeed" ); /*< How many string code may write developer in day */
OPTION_NAME CODE_QUALITY( L"codeQuality" );/*< What quality we have on game end.Percent */
OPTION_NAME KNOWLEDGE_LEVEL( L"knowledgeLevel" );/*< ������� ������. �� ����� ��������� ������� �������� ����� �������� ��������� ����*/
OPTION_NAME TALANT( L"talant" ); /*< ������. �� ����� ��������� ������� �������� ����� �������� ����*/
OPTION_NAME STAMINA( L"stamina" );/*< ���������. ��� �������� ���� 90% ���������� �� codeSpeed_, ��� ��������� ��������� �������� */
OPTION_NAME MAXMOOD( L"maxMood" );/*! ������������ ������� ����������, �������������� � ������ ��� */
OPTION_NAME MOOD( L"mood" );/*< ����������. ��� ������ ���������� ����� ��������� */
OPTION_NAME POPULARITY( L"popularity" );/*< ���������� ������������ */
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

	KNOWLEDGE_MAP genrePreferences_; /*< ������������ � �����, ������ �������� �� �������� */
	KNOWLEDGE_MAP genreExperience_;  /*< ���� ��������� ���, ������ �� ���� ���������� ����������� */
	KNOWLEDGE_MAP knowledges_;		/*< ������� ������ ���������� */
	WORKS works_;
	MODIFICATORS modificators_;
	RELATION_MAP _relations;	
};

typedef IUser* PUser;

}//namespace nrp

#endif