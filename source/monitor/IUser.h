#pragma once
#include "nrpConfig.h"

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
OPTION_NAME WANTMONEY( "wantMoney" ); /*! ��������, ������� ����� ������� ���� */
OPTION_NAME STABILITY("stability");/*< �������� ������� ��������� */
OPTION_NAME CHARACTER( "character" ); /*< �������� ��������� */
OPTION_NAME CONTRACTMONEY( "contractMoney" ); /*! ��������� ����� ��� ���������� ���������*/
OPTION_NAME MAXSUPPORTUSER( "maxSupportUser" ); /*! ���������� �����, ������� ����� ��������� ���*/
OPTION_NAME INVENTIONABILITY( "inventionAbility" ); /*! ����������� � ������������*/
OPTION_NAME USERSTATE( "userState" ); /*! ��������� ������������ "������", "�����" */
OPTION_NAME ROOMSTATE( "roomState" ); /*! �������, � ������� ��������� ������� */
OPTION_NAME PARENTCOMPANY( "parentCompany" ); /*! �������� � ������� �������� ������� */
OPTION_NAME MAXHOLIDAY( "maxHoliday" );	 /*! ������������ ���������� ���� ������� ��� ����� �������� */
OPTION_NAME CURRENTHOLIDAY( "currentHoliday" ); /*! ���������� ���������� ���� ������� */
OPTION_NAME HANGRY( "hangry" ); /*! ������ ������*/
OPTION_NAME COLLECTIVINTEGRATION( "collectivintegration" ); /*! ������ �������� � ����������*/
OPTION_NAME TIMEMANAGMENT( "timeManagment" ); /*!  ���������� ��������, �� ����� ��������� ������� �������� ����� ����������� �� ���������� */

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

	KNOWLEDGE_MAP genrePreferences_; /*< ������������ � �����, ������ �������� �� �������� */
	KNOWLEDGE_MAP genreExperience_;  /*< ���� ��������� ���, ������ �� ���� ���������� ����������� */
	KNOWLEDGE_MAP knowledges_;		/*< ������� ������ ���������� */
	USERACTION_LIST peopleFeels_; /*< ��������� � ����������� ������ */
	TECH_LIST techWorks_;
};

typedef IUser* PUser;

}//namespace nrp