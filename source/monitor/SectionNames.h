#pragma once
#include <string>

namespace nrp
{

typedef const std::string SECTION_NAME;
	
SECTION_NAME SECTION_OPTIONS( "options" );
SECTION_NAME SECTION_PROPERTIES( "properties" );
SECTION_NAME SECTION_COMPANIES( "companies" );
SECTION_NAME SECTION_INVENTIONS( "inventions" );
SECTION_NAME SECTION_PORTFELLE( "portfelle" );
SECTION_NAME SECTION_TECHS( "techs" );
SECTION_NAME SECTION_USERS( "users" );
SECTION_NAME SECTION_GENRES( "genres" );
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

#define KEY_COMPANY( index ) "company_" + conv::ToStr( index )
#define KEY_INVENTION( index ) "invention_" + conv::ToStr( index )
#define KEY_TECH( index ) "tech_" + conv::ToStr( index )
#define KEY_GENRE( index ) "genre_" + conv::ToStr( index )
#define KEY_WORK( index ) "work_" + conv::ToStr( index )
#define KEY_REKLAME( index ) "reklame_" + conv::ToStr( index )
#define KEY_PROJECT( index ) "project_" + conv::ToStr( index )
#define KEY_ENGINE( index ) "engine_" + conv::ToStr( index )
#define KEY_GAME( index ) "game_" + conv::ToStr( index )
#define KEY_USER( index ) "user_" + conv::ToStr( index )
#define KEY_TYPE( index ) "type_" + conv::ToStr( index )
#define KEY_MODULE( index ) "module_" + conv::ToStr( index )
#define KEY_SCREENSHOT(index) "screenShot" + conv::ToStr(index)
#define KEY_ITEM(index) "item_"+conv::ToStr(index)
#define KEY_ADDON(index) "addon_" + conv::ToStr( index )

}//end namespace nrp