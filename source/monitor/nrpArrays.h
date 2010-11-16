#pragma once

#include <irrArray.h>
#include <irrMap.h>
#include "nrpText.h"

namespace nrp
{

class INrpProject;
class CNrpGame;
class CNrpGameEngine;
class IUser;
class CNrpTechnology;
class CNrpDevelopGame;
class CNrpGameProject;
class CNrpInvention;
class INrpDevelopProject;
class CNrpRetailer;
class CNrpScreenshot;
class CNrpCompany;
class CNrpProjectModule;
class INrpConfig;
class IWorkingModule;
class IUserAction;
class IModificator;
class CNrpRelation;
class CNrpLoan;
class CNrpReklameWork;
class CNrpDiskMachine;

typedef core::array< CNrpLoan* > LOANS;
typedef core::array< CNrpGameEngine* > ENGINES;
typedef core::array< IUser* > USERS;
typedef core::array< CNrpTechnology* > TECHS;
typedef core::array< INrpProject* > PROJECTS;
typedef core::array< INrpDevelopProject* > DEVPROJECTS;
typedef core::array< CNrpGame* > GAMES;
typedef core::array< INrpConfig* > OBJECTS;
typedef core::array< CNrpInvention* > INVENTIONS;
typedef core::array< CNrpRetailer* > RETAILERS;
typedef core::array< CNrpScreenshot* > SCREENSHOTS; 
typedef core::array< CNrpCompany* > COMPANIES;
typedef core::array< CNrpProjectModule* > MODULES;
typedef core::array< NrpText > STRINGS;
typedef core::array< IWorkingModule* > WORKS;
typedef core::array< IUserAction* > USERACTIONS;
typedef core::array< CNrpReklameWork* > REKLAMES;
typedef core::array< CNrpDiskMachine* > DISKMACHINES;
typedef core::array< IModificator* > MODIFICATORS;

typedef core::map< NrpText, CNrpRelation* > RELATION_MAP;
typedef core::map< s32, s32 > REQUIRE_MAP;
typedef core::map< int, int > KNOWLEDGE_MAP;

template< typename ARRAY_TYPE, typename OBJECT_TYPE >  
OBJECT_TYPE* FindByName( const ARRAY_TYPE& arrayT, const NrpText& someName )
{
	for( u32 pos=0; pos < arrayT.size(); pos++ )
	{
		if( arrayT[ pos ]->GetString( NAME ) == someName  )
			return arrayT[ pos ];
	}

	return NULL;
}

template< typename ARRAY_TYPE, typename OBJECT_TYPE >  
OBJECT_TYPE* FindByNameAndIntName( const ARRAY_TYPE& arrayT, const NrpText& someName )
{
	for( u32 pos=0; pos < arrayT.size(); pos++ )
	{
		if( arrayT[ pos ]->GetString( NAME ) == someName || arrayT[ pos ]->GetString( INTERNAL_NAME ) == someName  )
			return arrayT[ pos ];
	}

	return NULL;
}

template< typename MAPTYPE >
void CopyMapTo( MAPTYPE& dst, const MAPTYPE& src )
{
	MAPTYPE& pm = const_cast< MAPTYPE& >( src );
	for( MAPTYPE::Iterator i=pm.getIterator(); !i.atEnd(); i++ )
		 dst.insert( i->getKey(), i->getValue() );
}

template< typename ARRAYT >
void AddArrayTo( ARRAYT& dst, const ARRAYT& src )
{
	ARRAYT& pm = const_cast< ARRAYT& >( src );
	for( u32 i=0; i < src.size(); i++ )
		 dst.push_back( src[ i ] );
}

}//end namespace nrp