#pragma once
#include "nrpConfig.h"

namespace nrp
{

OPTION_NAME CODE_SPEED( "codeSpeed" ); /*< How many string code may write developer in day */
OPTION_NAME CODE_QUALITY( "codeQuality" );/*< What quality we have on game end.Percent */
OPTION_NAME KNOWLEDGE_LEVEL( "knowledgeLevel" );/*< ������� ������. �� ����� ��������� ������� �������� ����� �������� ��������� ����*/
OPTION_NAME TALANT( "talant" ); /*< ������. �� ����� ��������� ������� �������� ����� �������� ����*/
OPTION_NAME STAMINA( "stamina" );/*< ���������. ��� �������� ���� 90% ���������� �� codeSpeed_, ��� ��������� ��������� �������� */
OPTION_NAME MOOD( "mood" );/*< ����������. ��� ������ ���������� ����� ��������� */
OPTION_NAME POPULARITY( "popularity" );/*< ���������� ������������ */
OPTION_NAME SALARY( "salary" );/*< �������� ������������*/
OPTION_NAME WANTMONEY( "wantMoney" ); /*! ��������, ������� ����� ������� ���� */
OPTION_NAME STABILITY("stability");/*< �������� ������� ��������� */
OPTION_NAME CHARACTER( "character" ); /*< �������� ��������� */
OPTION_NAME CONTRACTMONEY( "contractMoney" ); /*! ��������� ����� ��� ���������� ���������*/

class IUserAction;
class CNrpTechnology;

class IUser : public INrpConfig
{
public:
	IUser(const char* className, const char* systemName );
	void SetSkill( int typen, int valuel ); 
	void SetSkill( std::string name, int valuel );
	void AddTechWork( CNrpTechnology* techWork );
	CNrpTechnology* GetTechWork( int index );
	void RemoveTechWork( CNrpTechnology* techWork );
	~IUser(void);

private:         			
	void Load_( char* file_name ) {}
	void CalculateWantSalary_();
	void CalculateKnowledgeLevel_();

	typedef std::map< std::string, int > NAMEVALUE_MAP;
	typedef std::map< int, int > KNOWLEDGE_LIST;
	typedef std::vector< CNrpTechnology* > TECH_LIST;

	NAMEVALUE_MAP genrePreferences_; /*< ������������ � ����� */
	NAMEVALUE_MAP genreExperience_;  /*< ���� ��������� ���*/
	KNOWLEDGE_LIST knowledges_;		/*< ������� ������ ���������� */
	std::map< std::string, IUserAction* > peopleFeels_; /*< ��������� � ����������� ������ */
	TECH_LIST techWorks_;
};

typedef IUser* PUser;

}//namespace nrp