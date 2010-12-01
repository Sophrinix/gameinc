// nerpa_sc.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "nerpa_sc.h"
#include "nrpEngine.h"
#include "NrpApplication.h"
#include "Logger.h"

using namespace nrp;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	Logger::Instance();

	Log(LOG) << "Система логов запущена!" << term;
	CNrpEngine::Instance();
	
	CNrpApplication::Instance();
	CNrpApplication::Instance().Init();

	CNrpEngine::Instance().Run();

	Logger::Stop();
	//GPrintUndeletedSystemObject();
}