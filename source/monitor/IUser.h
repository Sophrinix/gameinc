#ifndef NRP_USER_H
#define NRP_USER_H

#include "nrpConfig.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME CODE_SPEED( "codeSpeed" ); /*< How many string code may write developer in day */
OPTION_NAME CODE_QUALITY( "codeQuality" );/*< What quality we have on game end.Percent */
OPTION_NAME KNOWLEDGE_LEVEL( "knowledgeLevel" );/*< ������� ������. �� ����� ��������� ������� �������� ����� �������� ��������� ����*/
OPTION_NAME TALANT( "talant" ); /*< ������. �� ����� ��������� ������� �������� ����� �������� ����*/
OPTION_NAME STAMINA( "stamina" );/*< ���������. ��� �������� ���� 90% ���������� �� codeSpeed_, ��� ��������� ��������� �������� */
OPTION_NAME MAXMOOD( "maxMood" );/*! ������������ ������� ����������, �������������� � ������ ��� */
OPTION_NAME MOOD( "mood" );/*< ����������. ��� ������ ���������� ����� ��������� */
OPTION_NAME POPULARITY( "popularity" );/*< ���������� ������������ */
OPTION_NAME SALARY( "salary" );/*< �������� ������������*/
OPTION_NAME WANTMONEY( "wantMoney" ); /*! ��������, ������� ����� ������� �������� */
OPTION_NAME STABILITY("stability");/*< �������� ������� ��������� */
OPTION_NAME CHARACTER( "character" ); /*< �������� ��������� */
OPTION_NAME CONTRACTMONEY( "contractMoney" ); /*! ��������� ����� ��� ���������� ���������*/
OPTION_NAME MAXSUPPORTUSER( "maxSupportUser" ); /*! ���������� �����, ������� ����� ��������� ���*/
OPTION_NAME INVENTIONABILITY( "inventionAbility" ); /*! ����������� � ������������*/
OPTION_NAME USERSTATE( "userState" ); /*! ��������� ������������ "������", "�����" */
OPTION_NAME ROOMSTATE( "roomState" ); /*! �������, � ������� ��������� ������� */
OPTION_NAME MAXHOLIDAY( "maxHoliday" );	 /*! ������������ ���������� ���� ������� ��� ����� �������� */
OPTION_NAME CURRENTHOLIDAY( "currentHoliday" ); /*! ���������� ���������� ���� ������� */
OPTION_NAME HANGRY( "hangry" ); /*! ������ ������*/
OPTION_NAME COLLECTIVINTEGRATION( "collectivintegration" ); /*! ������ �������� � ����������*/
OPTION_NAME TIMEMANAGMENT( "timeManagment" ); /*!  ���������� ��������, �� ����� ��������� ������� �������� ����� ����������� �� ���������� */
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
	IUser(const NrpText& className, const NrpText& systemName );

	void SetSkill( int typen, int valuel ); 
	void SetSkill( const NrpText& name, int valuel );
	int GetSkill( int typen );
	
	int GetGenreExperience( int typen );
	void SetGenreExperience( int typen, int valuel );
	void IncreaseExperience( int techGroup, int grow );

	int GetGenrePreferences( int typen );
	void SetGenrePreferences( int typen, int valuel );
	
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