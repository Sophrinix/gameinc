#pragma once
#include "nrpConfig.h"
#include <map>
#include <vector>
#include <string>

namespace nrp
{

OPTION_NAME CEO("ceo");
OPTION_NAME ENGINES_NUMBER( "engineNumber" );
OPTION_NAME TECHS_NUMBER( "techsNumber" );

class INrpProject;
class CNrpGame;
class CNrpGameEngine;
class IUser;
class CNrpTechnology;
class CNrpGameProject;

class CNrpCompany : public INrpConfig
{
public:
	typedef std::vector< CNrpGameEngine* > ENGINE_LIST;
	typedef std::map< std::string, CNrpTechnology* > TECH_MAP;

	CNrpCompany(const char* name);
	~CNrpCompany(void);

	void AddGameEngine( CNrpGameEngine* ptrEng );
	CNrpGameEngine* GetGameEngine( int index );
	CNrpTechnology* GetTech( int index );
	CNrpGameProject* AddGameProject( CNrpGameProject* ptrProject );
	void AddUser( IUser* user );

private:
	void Load_( char* file_name ) {}
	
	std::map< std::string, INrpProject* > projects_; 
	ENGINE_LIST engines_;
	TECH_MAP technologies_;
	std::map< std::string, CNrpGame* > games_;
	std::map< std::string, IUser* > employers_;
}; 

typedef CNrpCompany* PNrpCompany;

}//namespace nrp