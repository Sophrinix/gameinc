#pragma once
#include "NrpText.h"

namespace nrp
{

typedef const NrpText SECTION_NAME;
	
SECTION_NAME SECTION_OPTIONS( L"options" );
SECTION_NAME SECTION_PROPERTIES( L"properties" );
SECTION_NAME SECTION_SHARES( L"shares" );
SECTION_NAME SECTION_COMPANIES( L"companies" );
SECTION_NAME SECTION_INVENTIONS( L"inventions" );
SECTION_NAME SECTION_PORTFELLE( L"portfelle" );
SECTION_NAME SECTION_TECHS( L"techs" );
SECTION_NAME SECTION_USERS( L"users" );
SECTION_NAME SECTION_GENRES( L"genres" );
SECTION_NAME SECTION_RECENSE( L"recense" );
SECTION_NAME SECTION_VIDEO( L"video" );
SECTION_NAME SECTION_SOUND( L"sound" );
SECTION_NAME SECTION_LANGUGAGES( L"languages" );
SECTION_NAME SECTION_WORKS( L"works");
SECTION_NAME SECTION_REKLAMES( L"reklames" );
SECTION_NAME SECTION_PROJECTS( L"projects" );
SECTION_NAME SECTION_DEVPROJECTS( L"devProjects" );
SECTION_NAME SECTION_GAMES( L"games" );
SECTION_NAME SECTION_PLATFORMS( L"platforms" ); 
SECTION_NAME SECTION_ENGINES( L"engines" );
SECTION_NAME SECTION_FUTURE_TECH( L"nexttechs" );
SECTION_NAME SECTION_REQUIRE_TECH( L"techRequire" );
SECTION_NAME SECTION_REQUIRE_SKILL( L"skillRequire" );
SECTION_NAME SECTION_MODULES( L"modules" );
SECTION_NAME SECTION_ADDONS( L"addons" );
SECTION_NAME SECTION_IMAGES( L"images" );
SECTION_NAME SECTION_BOXIMAGES( L"imagesBox" );
SECTION_NAME SECTION_GENREPREFERENCE( L"genrePreference" );
SECTION_NAME SECTION_GENREEXPIRIENCE( L"genreExperience" );
SECTION_NAME SECTION_KNOWLEDGES( L"knowledges" );

NrpText CreateKeyImage( int index );
NrpText CreateKeyBoxImage( int index );

NrpText CreateKeyCompany( int index );
NrpText CreateKeyInvention( int index );
NrpText CreateKeyTech( int index );
NrpText CreateKeyGenre( int index );
NrpText CreateKeyRecense( int index );

NrpText CreateKeyProject( int index );
NrpText CreateKeyEngine( int index );
NrpText CreateKeyGame( int index );

NrpText CreateKeyType( int index );
NrpText CreateKeyModule( int index );
NrpText CreateKeyScreenshot( int index);
NrpText CreateKeyItem( int index);
NrpText CreateKeyAddon( int index);

NrpText CreateKeyUser( int index );
NrpText CreateKeyWork( int index );
NrpText CreateKeyReklame( int index );
NrpText CreateKeyPlatform( int index );
NrpText CreateSectionStep( int index );

}//end namespace nrp