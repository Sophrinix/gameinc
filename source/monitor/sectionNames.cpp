#include "StdAfx.h"
#include "SectionNames.h"

namespace nrp
{
NrpText CreateKeyWork( int index ) 
{ 
	return NrpText("work_") + NrpText( index ); 
}

NrpText CreateKeyReklame( int index ) 
{
	return NrpText("reklame_") + NrpText( index ); 
}

NrpText CreateKeyImage( int index ) { return NrpText("image_")+ NrpText(index); }
NrpText CreateKeyBoxImage( int index ) { return NrpText("boxImage_")+NrpText(index); }
NrpText CreateKeyCompany( int index ) { return NrpText("company_") + NrpText( index ); }
NrpText CreateKeyInvention( int index ) { return NrpText( "invention_" ) + NrpText( index ); }
NrpText CreateKeyTech( int index ) { return NrpText("tech_") + NrpText( index ); }
NrpText CreateKeyGenre( int index ) { return NrpText("genre_") + NrpText( index ); }

NrpText CreateKeyProject( int index ) { return NrpText("project_") + NrpText( index ); }
NrpText CreateKeyEngine( int index ) { return NrpText("engine_") + NrpText( index ); }
NrpText CreateKeyGame( int index ) { return NrpText("game_") + NrpText( index ); }
NrpText CreateKeyPlatform( int index ) { return NrpText("platform_") + NrpText( index ); }

NrpText CreateKeyType( int index ) { return NrpText("type_") + NrpText( index ); }
NrpText CreateKeyModule( int index ) { return NrpText("module_") + NrpText( index ); }
NrpText CreateKeyScreenshot( int index) { return NrpText("screenShot") + NrpText(index); }
NrpText CreateKeyItem( int index) { return NrpText("item_")+NrpText(index); }
NrpText CreateKeyAddon( int index) { return NrpText("addon_") + NrpText( index ); }

NrpText CreateKeyUser( int index ) { return NrpText("user_") + NrpText( index ); }

}