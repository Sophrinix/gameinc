#pragma once
#include "nrpConfig.h"
#include "nrpArrays.h"
#include "timeHelpers.h"

namespace nrp
{ 
OPTION_NAME CEO( L"ceo");
OPTION_NAME ENGINES_NUMBER( L"engineNumber" );
OPTION_NAME PROJECTNUMBER( L"projectNumber" );
OPTION_NAME GAMENUMBER( L"gameNumber" );
OPTION_NAME OBJECTSINPORTFELLE( L"objectInPortfelle" );
OPTION_NAME DEVELOPPROJECTS_NUMBER( L"developProjectsNumber" );
OPTION_NAME INVENTIONSNUMBER( L"inventionsNumber" );
OPTION_NAME MONEY_ON_PIE( L"moneyOnPie" );   //доход на акцию в денежном значении
OPTION_NAME PIE_COST( L"pieCost" );
OPTION_NAME PIE_NUMBER( L"pieNumber" );
OPTION_NAME PROFIT_LASTYEAR( L"profitLastYear" );
OPTION_NAME DIVIDEND( L"dividend" );		//доход на акции в процентном отношении к стоимости акции
OPTION_NAME BOOKKEEPING( L"bookKeeping" );
OPTION_NAME SELF_PIE_NUMBER( L"self_pie" );

class CNrpCompany : public INrpConfig
{
public:
	CNrpCompany( const NrpText& name, CNrpUser* ceo );
	CNrpCompany( const NrpText& fileName );
	~CNrpCompany(void);

	void AddGameEngine( CNrpGameEngine* ptrEng );
	CNrpGameEngine* GetGameEngine( int index ) const;
	CNrpGameEngine* GetGameEngine( const NrpText& name ) const;

	CNrpTechnology* GetTechnology( u32 index ) const;
	CNrpTechnology* GetTechnology( const NrpText& name ) const;
	void AddTechnology( CNrpTechnology* tech );
	INrpConfig* GetFromPortfelle( size_t index ) const;
	void RemoveFromPortfelle( const INrpConfig* ptrObject );
	void AddToPortfelle( INrpConfig* const ptrObject );
	float GetUserModificatorForGame( CNrpGame* game );

	void AddDevelopProject( INrpDevelopProject* ptrDevProject );
	INrpDevelopProject* GetDevelopProject( const NrpText& name ) const;
	INrpDevelopProject* GetDevelopProject( size_t index ) const;
	void RemoveDevelopProject( const NrpText& name );

	void AddProject( INrpProject* ptrProject );
	void RemoveProject( const NrpText& name );
	INrpProject* GetProject( const NrpText& name ) const;
	INrpProject* GetProject( size_t index ) const;

	void AddInvention( CNrpInvention* const inv );
	void InventionReleased( const CNrpInvention* inv );
	CNrpInvention* GetInvention( const NrpText& name );
	CNrpInvention* GetInvention( int index );
	void RemoveInvention( CNrpInvention* inv );

	void AddUser( CNrpUser* user );
	void RemoveUser( const NrpText& name );
	CNrpUser* GetUser( int index ) const;
	CNrpUser* GetUser( const NrpText& name ) const;
	
	void AddGame( CNrpGame* game );
	CNrpGame* GetGame( const NrpText& gameName ) const;
	CNrpGame* GetGame( size_t index ) const;
	CNrpGame* CreateGame( CNrpDevelopGame* ptrProject );

	void BeginNewHour( const NrpTime& time );
	void BeginNewDay( const NrpTime& time );
	void BeginNewMonth( const NrpTime& time );
	void BeginNewYear( const NrpTime& time );

	NrpText Save( const NrpText& saveFolder );
	void Load( const NrpText& loadFolder );

	static NrpText ClassName();
private:
	PROJECTS _projects; 
	ENGINES _engines;
	TECHS _technologies;
	DEVPROJECTS _devProjects;
	GAMES _games;
	USERS _employers;
	OBJECTS _portfelle;
	INVENTIONS _inventions;

	void _PaySalaries();
	void _UpdateGameProjectState();
	void _InitialyzeOptions();
	void _LoadArray( const NrpText& section, const NrpText& fileName, const NrpText& condition );
}; 

typedef CNrpCompany* PNrpCompany;
}//namespace nrp