--определение глобальных переменных для работы
guienv = NrpGetGuiEnvironment()
applic = NrpGetApplication()
driver = NrpGetVideoDriver()
sceneManager = NrpGetSceneManager()
browser = NrpGetBrowser()
autoscript = CLuaAutoScript()

scrWidth, scrHeight = driver:GetScreenSize()
--конец секции глобальных переменных

