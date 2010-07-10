#pragma once
#include "nrpConfig.h"
#include "NrpUserModificator.h"

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
OPTION_NAME MAXHOLIDAY( "maxHoliday" );	 /*! ������������ ���������� ���� ������� ��� ����� �������� */
OPTION_NAME CURRENTHOLIDAY( "currentHoliday" ); /*! ���������� ���������� ���� ������� */
OPTION_NAME HANGRY( "hangry" ); /*! ������ ������*/
OPTION_NAME COLLECTIVINTEGRATION( "collectivintegration" ); /*! ������ �������� � ����������*/
OPTION_NAME TIMEMANAGMENT( "timeManagment" ); /*!  ���������� ��������, �� ����� ��������� ������� �������� ����� ����������� �� ���������� */

class IUserAction;
class CNrpTechnology; 
class CNrpCompany;
class IModificator;
class CNrpProjectModule;

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
	
	void AddWork( CNrpProjectModule* techWork, bool inLoad );

	CNrpProjectModule* GetWork( int index );
	void RemoveWork( CNrpProjectModule* techWork );

	template< class R > R GetValueA( std::string name )
	{
		R paramValue = INrpConfig::GetValue<R>( name );
		MODIFICATOR_LIST::iterator pIter = modificators_.begin();

		for( ; pIter < modificators_.end(); pIter++ )
			if( ((CNrpUserModificator<R>*)*pIter)->GetName() == name )
				paramValue += ((CNrpUserModificator<R>*)*pIter)->GetValue();

		return paramValue;
	}

	void AddModificator( IModificator* ptrModificator );

	void BeginNewHour( const SYSTEMTIME& time );
	void BeginNewDay( const SYSTEMTIME& time );

	~IUser(void);
	void Save( std::string folderPath );
	void Load( std::string fileName );

private:         			
	template< class R > R& GetValue( std::string name ) { return INrpConfig::GetValue<R>( name ); }

	void Load_( char* file_name ) {}
	void CalculateWantSalary_();
	void CalculateKnowledgeLevel_();

	void RemoveOldModificators_( const SYSTEMTIME& time );
	typedef std::map< int, int > KNOWLEDGE_MAP;
	typedef std::vector< CNrpProjectModule* > WORK_LIST;
	typedef std::vector< IUserAction* > USERACTION_LIST;
	typedef std::vector< IModificator* > MODIFICATOR_LIST;

	KNOWLEDGE_MAP genrePreferences_; /*< ������������ � �����, ������ �������� �� �������� */
	KNOWLEDGE_MAP genreExperience_;  /*< ���� ��������� ���, ������ �� ���� ���������� ����������� */
	KNOWLEDGE_MAP knowledges_;		/*< ������� ������ ���������� */
	USERACTION_LIST peopleFeels_; /*< ��������� � ����������� ������ */
	WORK_LIST works_;
	MODIFICATOR_LIST modificators_;
};

typedef IUser* PUser;

}//namespace nrp