#pragma once
#include "nrpConfig.h"

namespace nrp
{

const std::string USER_NAME( "name" );
const std::string CODE_SPEED( "codeSpeed" ); /*< How many string code may write developer in day */
const std::string CODE_QUALITY( "codeQuality" );/*< What quality we have on game end.Percent */
const std::string KNOWLEDGE_LEVEL( "knowledgeLevel" );/*< ������� ������. �� ����� ��������� ������� �������� ����� �������� ��������� ����*/
const std::string TALANT( "talant" ); /*< ������. �� ����� ��������� ������� �������� ����� �������� ����*/
const std::string STAMINA( "stamina" );/*< ���������. ��� �������� ���� 90% ���������� �� codeSpeed_, ��� ��������� ��������� �������� */
const std::string MOOD( "mood" );/*< ����������. ��� ������ ���������� ����� ��������� */
const std::string POPULARITY( "popularity" );/*< ���������� ������������ */
const std::string SALARY( "salary" );/*< �������� ������������*/
const std::string STABILITY("stability");/*< �������� ������� ��������� */

class IUser : public INrpConfig
{
public:
	IUser(void);
	~IUser(void);
private:         			
	std::map< std::string, int > genrePreferences_; /*< ������������ � ����� */
	std::map< std::string, int > genreExperience_;  /*< ���� ��������� ���*/		
};

}//namespace nrp