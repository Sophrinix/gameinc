#pragma once
#include "NrpText.h"

namespace nrp
{

typedef const NrpText SECTION_NAME;
	
SECTION_NAME SECTION_OPTIONS( "options" );
SECTION_NAME SECTION_PROPERTIES( "properties" );
SECTION_NAME SECTION_COMPANIES( "companies" );
SECTION_NAME SECTION_INVENTIONS( "inventions" );
SECTION_NAME SECTION_PORTFELLE( "portfelle" );
SECTION_NAME SECTION_TECHS( "techs" );
SECTION_NAME SECTION_USERS( "users" );
SECTION_NAME SECTION_GENRES( "genres" );
SECTION_NAME SECTION_VIDEO( "video" );
SECTION_NAME SECTION_SOUND( "sound" );
SECTION_NAME SECTION_WORKS("works");
SECTION_NAME SECTION_REKLAMES( "reklames" );
SECTION_NAME SECTION_PROJECTS( "projects" );
SECTION_NAME SECTION_DEVPROJECTS( "devProjects" );
SECTION_NAME SECTION_GAMES( "games" );
SECTION_NAME SECTION_ENGINES( "engines" );
SECTION_NAME SECTION_FUTURE_TECH( "nexttechs" );
SECTION_NAME SECTION_REQUIRE_TECH( "techRequire" );
SECTION_NAME SECTION_REQUIRE_SKILL( "skillRequire" );
SECTION_NAME SECTION_MODULES( "modules" );
SECTION_NAME SECTION_ADDONS( "addons" );
SECTION_NAME SECTION_IMAGES( "images" );
SECTION_NAME SECTION_BOXIMAGES( "imagesBox" );

NrpText CreateKeyImage( int index );
NrpText CreateKeyBoxImage( int index );

NrpText CreateKeyCompany( int index );
NrpText CreateKeyInvention( int index );
NrpText CreateKeyTech( int index );
NrpText CreateKeyGenre( int index );

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

}//end namespace nrp