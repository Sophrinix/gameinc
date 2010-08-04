--определение глобальных переменных для работы
guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
applic = CLuaApplication( NrpGetApplication() )
driver = CLuaDriver( NrpGetVideoDriver() )
sceneManager = CLuaSceneManager( NrpGetSceneManager() )
browser = CLuaBrowser( NrpGetBrowser() )

scrWidth, scrHeight = driver:GetScreenSize()
--конец секции глобальных переменных

