// nerpa_sc.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "nerpa_sc.h"
#include "nrpVideoConfig.h"
#include "nrpHUDConfig.h"
#include "nrpEngine.h"
#include "NrpConsoleConfig.h"
#include "Logger.h"
#include "nrpScript.h"
#include "NrpHtmlEngineConfig.h"
#include "HTMLEngine.h"
#include "NrpPluginEngine.h"
#include "NrpApplication.h"
#include "NrpBank.h"
#include "nrpPlant.h"
#include "OpFileSystem.h"

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
	application[ BANK ] = &CNrpBank::Instance();

#ifdef _DEBUG
	application[ WORKDIR ] = OpFileSystem::UpDir( OpFileSystem::UpDir( __argv[ 0 ] ) );
#else
	application.SetValue<stringw>( WORKDIR, OpFileSystem::UpDir( __argv[ 0 ] ) );
#endif

	CNrpPluginEngine& p_engine = CNrpPluginEngine::Instance();

	//инициализация систем
	CNrpHUDConfig& hudConfig = CNrpHUDConfig::Instance();
	CNrpVideoConfig& videoConfig = CNrpVideoConfig::Instance();
	CNrpConsoleConfig& consoleConfig = CNrpConsoleConfig::Instance();
	CNrpHtmlEngineConfig& heConfig = CNrpHtmlEngineConfig::Instance();
	CNrpPlant& plant = CNrpPlant::Instance();
	CNrpScript& scriptSystem = CNrpScript::Instance();

	//ожидаем подгрузки видео
	v_engine.InitVideo();

	HTMLEngine& htmlEngine = HTMLEngine::Instance();
	v_engine.InitConsole();
	v_engine.Run();

	Logger::Stop();

	//GPrintUndeletedSystemObject();
}