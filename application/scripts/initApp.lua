require("elmid")
IncludeScript("logdefs")
IncludeScript( "NerpaActionType" )

--определение глобальных переменных для работы
guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
applic = CLuaApplication( NrpGetApplication() )
driver = CLuaDriver( NrpGetVideoDriver() )
sceneManager = CLuaSceneManager( NrpGetSceneManager() )

scrWidth, scrHeight = driver:GetScreenSize()
--конец секции глобальных переменных

NrpLoadLanguageFile( "localization/russian.lng" )

NrpLoadPlugins( "Plugins/" )

--загрузка сцены авторизации пользователя
NrpAddScene( "sceneLogin", "scripts/sceneLogin.lua" )

--загрузка рабочей сцены
NrpAddScene( "sceneWork", "scripts/sceneWork.lua" )
	
--устнановка начальной сцены
NrpSetNextScene( "sceneLogin" )

--test_OutputDebugString()