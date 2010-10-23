--определение глобальных переменных для работы
guienv = NrpGetGuiEnvironment()
applic = NrpGetApplication()
driver = NrpGetVideoDriver()
sceneManager = NrpGetSceneManager()
browser = NrpGetBrowser()
autoscript = CLuaAutoScript()

scrWidth, scrHeight = driver:GetScreenSize()
FADE_TIME = 200
AFADE_TIME = 205
REMOVE_ON_END = false
FADE_OUT = false
FADE_IN = true

alwaysOnTopWindow = {}

--конец секции глобальных переменных

