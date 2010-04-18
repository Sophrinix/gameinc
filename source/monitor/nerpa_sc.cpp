// nerpa_sc.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "nerpa_sc.h"
#include "nrpVideoConfig.h"
#include "nrpHUDConfig.h"
#include "NrpWorldConfig.h"
#include "NrpSkyConfig.h"
#include "nrpEngine.h"
#include "NrpConsoleConfig.h"
#include "Logger.h"
#include "nrpScript.h"
#include "NrpHtmlEngineConfig.h"
#include "HTMLEngine.h"
#include "NrpPluginEngine.h"
#include "NrpApplication.h"
#include "NrpBank.h"
using namespace nrp;
using namespace plugin;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	Logger& logSystem = Logger::Instance();
	Log(LOG) << "Система логов запущена!" << term;
	CNrpEngine& v_engine = CNrpEngine::Instance();
	CNrpApplication& application = CNrpApplication::Instance();
	application.SetValue<PNrpBank>( BANK, &CNrpBank::Instance() );
	application.SetValue<std::string>( FULLPATH, "" );
	CNrpPluginEngine& p_engine = CNrpPluginEngine::Instance();

	CNrpVideoConfig& videoConfig = CNrpVideoConfig::Instance();
	CNrpHUDConfig& hudConfig = CNrpHUDConfig::Instance();
	CNrpWorldConfig& worldConfig = CNrpWorldConfig::Instance();
	CNrpSkyConfig& skyConfig = CNrpSkyConfig::Instance();
	CNrpConsoleConfig& consoleConfig = CNrpConsoleConfig::Instance();
	CNrpHtmlEngineConfig& heConfig = CNrpHtmlEngineConfig::Instance();

	CNrpScript& scriptSystem = CNrpScript::Instance();
	scriptSystem.DoFile( "scripts/initApp.lua" );
	
	v_engine.SetConfig( &videoConfig );
	v_engine.SetConfig( &hudConfig );
	v_engine.SetConfig( &skyConfig );
	v_engine.SetConfig( &consoleConfig );

	v_engine.InitVideo();
	
	while( v_engine.GetDevice() == NULL )
		Sleep( 100 );

	HTMLEngine& htmlEngine = HTMLEngine::Instance();
	v_engine.InitConsole();
	v_engine.Run();

	Logger::Stop();

	//GPrintUndeletedSystemObject();
}