--определение глобальных переменных для работы
guienv = NrpGetGuiEnvironment()
applic = NrpGetApplication()
driver = NrpGetVideoDriver()
sceneManager = NrpGetSceneManager()
browser = NrpGetBrowser()
autoscript = CLuaAutoScript()
soundenv = applic.soundEngine
questenv = applic.questEngine

scrWidth, scrHeight = driver:GetScreenSize()
FADE_TIME = 200
AFADE_TIME = 205
REMOVE_ON_END = true
FADE_OUT = false
FADE_IN = true

--конец секции глобальных переменных

